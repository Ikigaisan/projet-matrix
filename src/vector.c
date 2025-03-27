#include "../headers/vector.h"

vector *init_vector(uint64_t m) {
    vector *v = (vector *)malloc(sizeof(vector));
    if (v == NULL) {
        fprintf(
            stderr,
            "Problème lors de l'allocation de l'espace mémoire pour un vecteur "
            ": %s\n",
            strerror(errno));
            free(v);
        exit(EXIT_FAILURE);
    }
    v->m = m;
    v->values = (double *)malloc(m * sizeof(double));
    if (v->values == NULL) {
        fprintf(
            stderr,
            "Problème lors de l'allocation de l'espace mémoire pour un vecteur "
            ": %s\n",
            strerror(errno));
        exit(EXIT_FAILURE);
    }
    for (uint64_t i = 0; i < m; i++) {
        v->values[i] = 0;
    }
    return v;
}

void free_vector(vector *v) {
    if (v != NULL) {
        free(v->values);  // Libère le tableau de valeurs
        free(v);          // Libère la structure elle-même
    }
}

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

void add_v_v(vector *x, vector *y, vector *z) {
    uint64_t m = x->m;

    if (x == NULL || y == NULL || z == NULL) {
        fprintf(stderr, "Erreur : l'un des vecteurs est NULL\n");
        return;
    }

    if (x->m != y->m || x->m != z->m) {
        fprintf(stderr, "Erreur : les tailles des vecteurs ne correspondent pas\n");
        return;
    }

    for (uint64_t i = 0; i < m; i++) {
        z->values[i] = x->values[i] + y->values[i];
    }
}
