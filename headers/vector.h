#include <errno.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifndef _VECTOR_H_
#define _VECTOR_H_

typedef struct {
    uint64_t m;     // La taille du vecteur
    double *values; // Les valeurs contenues dans le vecteur
} vector;

/**
 * Initialise un vecteur de taille m avec des zéros.
 *
 * @param uint64_t m la taille du vecteur
 *
 * @return un pointeur vers un vecteur de taille m initialisé avec des 0.
 */
vector *init_vector(uint64_t);

/**
 * Imprime sur la sortie standard le contenu d'un vecteur.
 *
 * @param vector* un pointeur vers le vecteur à imprimer
 *
 * @result le vecteur a été imprimé sur la sortie standard.
 */
void print_vector(vector *);

/**
 * Calcule la somme de deux vecteurs
 *
 * @param vector* x le premier vecteur
 * @param vector* y le second vecteur
 * @param vector* z un vecteur avec des valeurs quelconques dans lequel la somme
 * de x avec y sera stockée
 *
 * @result z = x + y
 */
void add_v_v(vector *, vector *, vector *);




/**
 * Calcule la soustraction de deux vecteurs
 *
 * @param vector* x le premier vecteur
 * @param vector* y le second vecteur
 * @param vector* z un vecteur avec des valeurs quelconques dans lequel la soustraction
 * de x avec y sera stockée
 *
 * @result z = x - y
 */
void sub_v_v(vector *, vector *, vector *);

/**
 * @brief Calule le produit scalaire entre deux vecteurs.
 * 
 * @param vector* x Pointeur vers le premier vecteur.
 * @param vector* y Pointeur vers le deuxième vecteur.
 * @param double* result Pointeur dans lequel stocker le résultat. 
 * 
 * @result result = x_i * y_i
 * 
 * @pre Les vecteurs x et y doivent être de la même taille.
 * 
 */
void dot_prod(vector *, vector *, double *);

void norm(vector *, double *);


#endif /* _VECTOR_H_ */