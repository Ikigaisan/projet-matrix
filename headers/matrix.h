#ifndef _MATRIX_H_
#define _MATRIX_H_

#include <errno.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "vector.h"  // On inclut vector.h au lieu de redéfinir vector

typedef struct {
    uint64_t m;      // Nombre de lignes de la matrice
    uint64_t n;      // Nombre de colonnes de la matrice
    double **values; // Les valeurs contenues dans la matrice
} matrix;

/**
 * Initialise une matrice de taille m x n avec des 0.
 *
 * @param uint64_t m le nombre de lignes de la matrice
 * @param uint64_t n le nombre de colonnes de la matrice
 *
 * @return un pointeur vers une matrice de taille m x n initialisée avec des 0.
 */
matrix *init_matrix(uint64_t, uint64_t);

/**
 * Imprime sur la sortie standard le contenu d'une matrice.
 *
 * @param matrix* un pointeur vers la matrice à imprimer
 *
 * @result la matrice a été imprimée sur la sortie standard.
 */
void print_matrix(matrix *);

/**
 * Calcule la somme de deux matrices
 *
 * @param matrix* A la première matrice
 * @param matrix* B la seconde matrice
 * @param matrix* C une matrice avec des valeurs quelconques dans laquelle la
 * somme de A avec B sera stockée
 *
 * @result C = A + B
 */
void add_m_m(matrix *, matrix *, matrix *);

/**
 * Résolution par substitution arrière.
 *
 * @param vector* b le vecteur de résultats
 * @param matrix* U la matrice triangulaire supérieure
 * @param vector* x le vecteur solution (résultat)
 */
void back_sub(vector *b, matrix *U, vector *x);

#endif // _MATRIX_H_
