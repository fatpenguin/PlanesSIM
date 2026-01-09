//
// Created by David Robertaud on 07/01/2026.
//

#include "PLANES_ERROR.h"

#include <stdio.h>
#include <string.h>


static const char *INSTANTIATION_ERRORS[] = {
    "Could not allocate a structure of arrays for planes",
    "Undefined error"
};

static const char *DESTROY_ERRORS[] = {
    "Given struct pointer is empty",
    "Undefined error"
};

static const char *BOOKING_ERRORS[] = {
    "There is not enough available planes",
    "Provided name is NULL",
    "Undefined error"
};


int PLANE_ERR_NUM = 0;

void planes_error(const char *function_name) {
    if (strcmp(function_name, "instantiate_planes") == 0) {
        if (PLANE_ERR_NUM > PLANES_INSTANT_ERR_COUNT - 1) {
            fprintf(stderr,"Unknown plane error %d", PLANE_ERR_NUM);
        }
        fprintf(stderr, "%s\n", INSTANTIATION_ERRORS[PLANE_ERR_NUM]);
    }
    else if (strcmp(function_name, "destroy_planes") == 0) {
        if (PLANE_ERR_NUM > PLANES_DESTROY_ERR_COUNT - 1) {
            fprintf(stderr,"Unknown plane error %d", PLANE_ERR_NUM);
        }
        fprintf(stderr, "%s\n", DESTROY_ERRORS[PLANE_ERR_NUM]);
    }
    else if (strcmp(function_name, "book_planes") == 0) {
        if (PLANE_ERR_NUM > PLANES_BOOKING_ERR_COUNT - 1) {
            fprintf(stderr,"Unknown plane error %d", PLANE_ERR_NUM);
            fprintf(stderr,"Unknown plane error %d", PLANE_ERR_NUM);
        }
        fprintf(stderr, "%s\n", BOOKING_ERRORS[PLANE_ERR_NUM]);
    }
    else {
        fprintf(stderr, "Unknown function\n");
    }
}