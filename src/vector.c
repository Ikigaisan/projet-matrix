#include "../headers/vector.h"
#include <math.h>
vector *init_vector(uint64_t m) {
    vector *v = (vector *)malloc(sizeof(vector));
    if (v == NULL) {
        fprintf(
            stderr,
            "Problème lors de l'allocation de l'espace mémoire pour un vecteur "
            ": %s\n",
            strerror(errno));
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
        free(v);
        exit(EXIT_FAILURE);
    }
    for (uint64_t i = 0; i < m; i++) {
        v->values[i] = 0;
    }
    return v;
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
    for (uint64_t i = 0; i < m; i++) {
        z->values[i] = x->values[i] + y->values[i];
    }
}

void sub_v_v(vector *x, vector *y, vector *z) {
    uint64_t m = x->m;
    for (uint64_t i = 0; i < m; i++) {
        z->values[i] = x->values[i] - y->values[i];
    }
}

void dot_prod(vector *x, vector *y, double *result){
    *result = 0.0;
    if(x->m != y->m){
        fprintf(stderr, "Erreur : les vecteurs doivent avoir la même taille !\n");
        return;
    }
    for(uint64_t i = 0; i<x->m; i++){
        *result += x->values[i] * y->values[i];
    }
}


void norm(vector *x, double *result){
    if (!x || !x->values || !result) {
        fprintf(stderr, "Erreur : pointeur NULL détecté dans norm()\n");
        return;
    }
    double s = 0;
    uint64_t m = x->m;
    for (uint64_t i = 0; i < m; i++){
        s += (x->values[i]) * (x->values[i]);
    }
    *result = sqrt(s); 
}

