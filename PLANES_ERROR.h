//
// Created by David Robertaud on 07/01/2026.

#ifndef PLANES_ERROR_H
#define PLANES_ERROR_H

typedef enum planes_status {
    SUCCESS = 0,
    ERROR = -1
} planes_status;


#define NO_ERROR 0

enum PLANES_INSTANT_ERROR_MAP {
    PLANE_ALLOCATION_FAILED,
    PLANES_INSTANT_ERR_COUNT
};

enum PLANES_DESTROY_ERROR_MAP {
    EMPTY_PLANES_POINTER,
    PLANES_DESTROY_ERR_COUNT
};

enum PLANES_BOOKING_ERROR_MAP {
    INVALID_NBR_BOOKING,
    INVALID_NAME_POINTER,
    PLANES_BOOKING_ERR_COUNT
};

extern int PLANE_ERR_NUM;

void planes_error();



#endif //PLANES_ERROR_H
