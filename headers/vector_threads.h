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

/**
 * @brief Utilise les threads pour ajouter deux vecteurs
 * 
 * @param arg* pointeur vers une structure thread_data_v_v qui contient :
 *  - Les indices de début et de fin start_idx et end_idx
 *  - Les pointeurs vers les vecteurs x y et z
 * 
 * @result z = x + y
 */
void add_v_v_thread(void *arg);


#endif /* _VECTOR_H_T */