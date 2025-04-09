#include <errno.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "vector.h"

#ifndef _VECTOR_H_T
#define _VECTOR_H_T

typedef struct {
    int start_idx;
    int end_idx;
    vector *x;
    vector *y;
    vector *z;
} thread_data_v_v;


#endif /* _VECTOR_H_T */