#include <errno.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <inttypes.h>
#include "vector.h"

#ifndef _MATRIX_H_T
#define _MATRIX_H_T

typedef struct {
    matrix* A;
    matrix* T;
    uint64_t start_row;
    uint64_t end_row;
} thread_data_transp;

/**
 * @brief Fonction exécutée par chaque thread pour transposer une partie de la matrice.
 *
 * @param arg Pointeur vers une structure thread_data_transp.
 * Cette structure contient les informations nécessaires pour la transposition :
 * - La matrice originale (A) et la matrice transposée (T).
 * - Les indices de début et de fin des lignes à traiter.
 *
 * @return void* Toujours NULL dans cette implémentation.
 *
 * @details Chaque thread transpose les lignes de la matrice originale (A) spécifiées par start_row et end_row
 * dans la matrice transposée (T).
 */
 void* transp_thread(void* arg);

 #endif // _MATRIX_H_T



