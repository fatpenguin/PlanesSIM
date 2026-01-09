//
// Created by David Robertaud on 07/01/2026.
//

#ifndef PLANES_H
#define PLANES_H

#include <stdbool.h>
#include <stdint.h>
#include <sys/_types/_size_t.h>

#include "PLANES_ERROR.h"

struct planes_t {
    int *distance;
    int *speed;
    int8_t *booker_id;
    bool *used;
    char **names;
    size_t nbr_planes;
    size_t available;
};


planes_status instantiate_planes(size_t number, struct planes_t **);
planes_status destroy_planes(struct planes_t **);
planes_status book_planes(int8_t booker_id, const char *names[], size_t number, struct planes_t **planes);



#endif //PLANES_H
