#include "../headers/vector.h"
#include <math.h>
#include <inttypes.h>

/**
 * Alloue et initialise un vecteur de taille m.
 *
 * Le vecteur est initialisé avec des zéros.
 *
 * @param m La taille du vecteur à allouer.
 * @return Un pointeur vers le vecteur alloué.
 */
vector *init_vector(uint64_t m) {
    vector *v = (vector *)malloc(sizeof(vector));
    if (v == NULL) {
        fprintf(
            stderr,
            "Problème lors de l'allocation de l'espace mémoire pour un vecteur : %s\n",
            strerror(errno));
        exit(EXIT_FAILURE);
    }
    v->m = m;
    v->values = (double *)malloc(m * sizeof(double));
    if (v->values == NULL) {
        fprintf(
            stderr,
            "Problème lors de l'allocation de l'espace mémoire pour un vecteur : %s\n",
            strerror(errno));
        free_vector(v);
        exit(EXIT_FAILURE);
    }
    // Initialisation à zéro de chaque élément
    for (uint64_t i = 0; i < m; i++) {
        v->values[i] = 0;
    }
    return v;
}

/**
 * Libère la mémoire occupée par un vecteur.
 *
 * @param v Le vecteur à libérer.
 */
void free_vector(vector *v) {
    if (v != NULL) {
        free(v->values);
        free(v);
    }
}

/**
 * Affiche le contenu d'un vecteur sur la sortie standard.
 *
 * @param v Le vecteur à afficher.
 */
void print_vector(vector *v) {
    if (v->m == 0) {
        printf("[]\n");
        return;
    }
    printf("[%f", v->values[0]);
    for (uint64_t i = 1; i < v->m; i++) {
        printf(" %f", v->values[i]);
    }
    printf("]\n");
}

/**
 * Additionne deux vecteurs x et y et stocke le résultat dans z.
 *
 * @param x Le premier vecteur.
 * @param y Le second vecteur.
 * @param z Le vecteur résultat.
 */
void add_v_v(vector *x, vector *y, vector *z) {
    uint64_t m = x->m;
    for (uint64_t i = 0; i < m; i++) {
        z->values[i] = x->values[i] + y->values[i];
    }
}

/**
 * Soustrait le vecteur y au vecteur x et stocke le résultat dans z.
 *
 * @param x Le vecteur de départ.
 * @param y Le vecteur à soustraire.
 * @param z Le vecteur résultat.
 */
void sub_v_v(vector *x, vector *y, vector *z) {
    uint64_t m = x->m;
    if(m != y->m){
        fprintf(stderr, "Erreur : les vecteurs doivent avoir la même taille !\n");
    }
    if(m != z->m){
        fprintf(stderr, "Erreur : les vecteurs doivent avoir la même taille !\n");
    }
    for (uint64_t i = 0; i < m; i++) {
        z->values[i] = x->values[i] - y->values[i];
    }
}

/**
 * Calcule le produit scalaire de deux vecteurs x et y.
 *
 * @param x Le premier vecteur.
 * @param y Le second vecteur.
 * @param result Pointeur sur le résultat du produit scalaire.
 */
void dot_prod(vector *x, vector *y, double *result){
    *result = 0.0;
    if(x->m != y->m){
        fprintf(stderr, "Erreur : les vecteurs doivent avoir la même taille !\n");
        exit(EXIT_FAILURE);
    }
    for(uint64_t i = 0; i < x->m; i++){
        *result += x->values[i] * y->values[i];
    }
}

/**
 * Calcule la norme (euclidienne) d'un vecteur x.
 *
 * @param x Le vecteur dont on calcule la norme.
 * @param result Pointeur sur la variable recevant la norme.
 */
void norm(vector *x, double *result){
    if (!x || !x->values || !result) {
        fprintf(stderr, "Erreur : pointeur NULL détecté dans norm()\n");
        exit(EXIT_FAILURE);
    }
    double s = 0;
    uint64_t m = x->m;
    for (uint64_t i = 0; i < m; i++){
        s += (x->values[i]) * (x->values[i]);
    }
    *result = sqrt(s); 
}
