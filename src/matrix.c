#include "../headers/matrix.h"
#include "../headers/vector.h"

#include <float.h>
#include <math.h>

matrix *init_matrix(uint64_t m, uint64_t n) {
    matrix *A = (matrix *)malloc(sizeof(matrix));
    if (A == NULL) {
        fprintf(stderr, "Erreur d'allocation pour la matrice : %s\n", strerror(errno));
        exit(EXIT_FAILURE);
    }
    A->m = m;
    A->n = n;
    A->values = (double **)malloc(m * sizeof(double *));
    if (A->values == NULL) {
        fprintf(stderr, "Erreur d'allocation pour les lignes de la matrice : %s\n", strerror(errno));
        free(A);
        exit(EXIT_FAILURE);
    }

    for (uint64_t i = 0; i < m; i++) {
        A->values[i] = (double *)malloc(n * sizeof(double));
        if (A->values[i] == NULL) {
            fprintf(stderr, "Erreur d'allocation pour une ligne de la matrice : %s\n", strerror(errno));

            // Libérer les lignes déjà allouées
            for (uint64_t k = 0; k < i; k++) {
                free(A->values[k]);
            }
            free(A->values);
            free(A);
            exit(EXIT_FAILURE);
        }

        // Initialiser les valeurs à 0
        memset(A->values[i], 0, n * sizeof(double));
    }
    return A;
}

void free_matrix(matrix *A) {
    if (A == NULL) return;

    // Libération des lignes
    for (uint64_t i = 0; i < A->m; i++) {
        free(A->values[i]);
    }

    // Libération du tableau de pointeurs et de la structure
    free(A->values);
    free(A);
}


void print_matrix(matrix *A) {
    if (A->m == 0) {
        printf("[[]]\n");
        return;
    }
    printf("[[%f", A->values[0][0]);
    for (uint64_t j = 1; j < A->n; j++) {
        printf(" %f", A->values[0][j]);
    }
    if (A->m == 1) {
        printf("]]\n");
        return;
    }
    printf("]\n");
    for (uint64_t i = 1; i < A->m; i++) {
        printf(" [%f", A->values[i][0]);
        for (uint64_t j = 1; j < A->n; j++) {
            printf(" %f", A->values[i][j]);
        }
        if (i == A->m - 1) {
            printf("]]\n");
            return;
        }
        printf("]\n");
    }
}

void add_m_m(matrix *A, matrix *B, matrix *C) {
    uint64_t m = A->m;
    uint64_t n = A->n;
    for (uint64_t i = 0; i < m; i++) {
        for (uint64_t j = 0; j < n; j++) {
            C->values[i][j] = A->values[i][j] + B->values[i][j];
        }
    }
}

void back_sub(vector*b, matrix *U, vector*x){
    uint64_t m = b->m;
    uint64_t infinité = 0;
    for (uint64_t i =0; i < m; i++ ){
        x -> values[i] =  b -> values[i];
    }
    // Cas particulier d'une seule inconnue
    if (m == 1) {
        if (fabs(U->values[0][0]) < DBL_EPSILON) {
            if (fabs(b->values[0]) < DBL_EPSILON) {
                // Infinité de solutions : assigner une valeur arbitraire
                x->values[0] = 1.0;
                fprintf(stderr, "Infinité de solutions\n");
            } else {
                // Aucune solution : division par zéro
                fprintf(stderr, "Aucune solution\n");
                //exit(1);
                return;
            }
        } else {
            // Cas standard : on résout x[0] = b[0] / U[0][0]
            x->values[0] = b->values[0] / U->values[0][0];
        }
        return;
    }
    if (m<1){
        fprintf(stderr, "La matrice n'est pas valide.\n");
        return;
    }
    for (int64_t i = m-1; i >= 0; i--){ //pas mettre uint64_t car le u impose que u tjrs positif
        for (int64_t j = m-1; j > i; j--){
            x -> values[i] -= U->values[i][j] * x->values[j]; 
        }
        if (fabs(U->values[i][i]) < DBL_EPSILON){ //çad =0
            if (fabs(x-> values[i]) < DBL_EPSILON){
                x-> values[i] = 1; //valeur aléatoire car infinité de solutions
                infinité ++;
            }
            else {
                fprintf(stderr, "Aucune solution\n");
                return;
            }
        }
        x-> values[i] /= U->values[i][i];
    }
    if (infinité>0){
        fprintf(stderr, "Infinité de solutions\n");
    }
}