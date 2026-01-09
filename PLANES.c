//
// Created by David Robertaud on 07/01/2026.
//

#include "PLANES.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "PLANES_ERROR.h"
#include "SETUP.h"

/// MEMORY ALLOCATION CODE
planes_status instantiate_planes(const size_t number, struct planes_t **planes) {
    size_t size =
         sizeof(struct planes_t) +
         number * sizeof(int) +           // posX
         number * sizeof(int) +           // posY
         number * sizeof(int8_t) +        // booker_id
         number * sizeof(bool) +          // used
         number * sizeof(char *) +        // tableau pointers names
         number * NAME_MAX_LENGTH;        // data names

    struct planes_t *temp = calloc(1, size);
    if (temp == NULL) {
        perror("calloc");
        PLANE_ERR_NUM = PLANE_ALLOCATION_FAILED;
        return ERROR;
    }
    char *p = (char *)(temp + 1);

    temp->distance = (int *)p;
    p += number * sizeof(int);

    temp->speed= (int *)p;
    p += number * sizeof(int);

    temp->booker_id = (int8_t *)p;
    p += number * sizeof(int8_t);

    temp->used = (bool *)p;
    p += number * sizeof(bool);

    temp->names = (char **)p;
    p += number * sizeof(char *);

    char *names_data = p;
    for (size_t i = 0; i < number; i++) {
        temp->names[i] = names_data + i * NAME_MAX_LENGTH;
    }

    temp->nbr_planes = number;
    temp->available = number;

    *planes = temp;
    return SUCCESS;
}

planes_status destroy_planes(struct planes_t **planes) {
    if (*planes == NULL) {
        PLANE_ERR_NUM = EMPTY_PLANES_POINTER;
        return ERROR;
    }
    free(*planes);
    *planes = NULL;
    return SUCCESS;
}
/// [END] MEMORY ALLOCATION CODE

/// BOOKING CODE
planes_status book_planes(const int8_t booker_id, const char *names[], size_t number, struct planes_t **planes) {
    struct planes_t *temp = *planes;
    if (number > temp->available) {
        PLANE_ERR_NUM = INVALID_NBR_BOOKING;
        return ERROR;
    }

    size_t index_start_search = 0;
    for (size_t i = 0; i < number; i++) {
        if (names[i] == NULL) {
            PLANE_ERR_NUM = INVALID_NAME_POINTER;
            return ERROR;
        }

        int found = 0;
        for (size_t j = index_start_search; j < temp->nbr_planes; j++) {
            if (!temp->used[j]) {
                strncpy(temp->names[j], names[i], NAME_MAX_LENGTH - 1);
                temp->names[j][NAME_MAX_LENGTH - 1] = '\0';
                index_start_search = j + 1;
                temp->used[j] = true;
                temp->booker_id[j] = booker_id;
                temp->available--;
                found = 1;
                break;
            }
        }

        if (!found) {
            PLANE_ERR_NUM = INVALID_NBR_BOOKING;
            return ERROR;
        }
    }

    return SUCCESS;
}
/// [END] BOOKING CODE
/// PLANE UPDATE
planes_status update_planes(struct planes_t *planes, int delta_t) {
    if (planes == NULL) {
        PLANE_ERR_NUM = EMPTY_PLANES_POINTER;
        return ERROR;
    }
    for (size_t i = 0; i < planes->nbr_planes; i++) {
        planes->distance[i] -= planes->speed[i] * delta_t & -(int)planes->used[i];
    }
    return SUCCESS;
}