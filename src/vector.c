#include "../headers/vector.h"
#include <math.h>
#include <inttypes.h>
#include "../headers/error.h"

#define SUCCESS 0


vector *init_vector(uint64_t m) {
    vector *v = (vector *)malloc(sizeof(vector));
    if (v == NULL) {
        handle_error(ERROR_ALLOC_STRUCT);
    }
    v->m = m;
    v->values = (double *)malloc(m * sizeof(double));
    if (v->values == NULL) {
        free_vector(v);
        handle_error(ERROR_ALLOC_VALUES);
    }
    // Initialisation à zéro de chaque élément
    for (uint64_t i = 0; i < m; i++) {
        v->values[i] = 0;
    }
    return v;
}


void free_vector(vector *v) {
    if (v != NULL) {
        free(v->values);
        free(v);
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


int add_v_v(vector *x, vector *y, vector *z) {
    uint64_t m = x->m;
    if(m != y->m || m != z->m) handle_error(ERROR_SIZE_MISMATCH);

    for (uint64_t i = 0; i < m; i++) {
        z->values[i] = x->values[i] + y->values[i];
    }
    return SUCCESS;
}


int sub_v_v(vector *x, vector *y, vector *z) {
    uint64_t m = x->m;
    if(m != y->m || m != z->m) handle_error(ERROR_SIZE_MISMATCH);

    for (uint64_t i = 0; i < m; i++) {
        z->values[i] = x->values[i] - y->values[i];
    }
    return SUCCESS;
}

int dot_prod(vector *x, vector *y, double *result){
    if (!x || !y || !result) handle_error(ERROR_NULL_POINTER);
    if (!x->values || !y->values) handle_error(ERROR_NULL_VALUES);

    *result = 0.0;
    if(x->m != y->m) handle_error(ERROR_SIZE_MISMATCH);

    for(uint64_t i = 0; i < x->m; i++){
        *result += x->values[i] * y->values[i];
    }
    return SUCCESS;
}

/**
 * Calcule la norme (euclidienne) d'un vecteur x.
 *
 * @param x Le vecteur dont on calcule la norme.
 * @param result Pointeur sur la variable recevant la norme.
 */
int norm(vector *x, double *result){
    if (!x || !result) handle_error(ERROR_NULL_POINTER);
    if (!x->values) handle_error(ERROR_NULL_VALUES);

    double s = 0;
    uint64_t m = x->m;
    for (uint64_t i = 0; i < m; i++){
        s += (x->values[i]) * (x->values[i]);
    }
    *result = sqrt(s); 

    return SUCCESS;
}
