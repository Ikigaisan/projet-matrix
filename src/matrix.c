#include "../headers/matrix.h"
#include "../headers/vector.h"
#include <inttypes.h>

matrix *init_matrix(uint64_t m, uint64_t n) {
    matrix *A = (matrix *)malloc(sizeof(matrix));
    if (A == NULL) {
        fprintf(stderr,
                "Problème lors de l'allocation de l'espace mémoire pour une "
                "matrice : %s\n",
                strerror(errno));               
        exit(EXIT_FAILURE);
    }
    A->m = m;
    A->n = n;
    A->values = (double **)malloc(m * sizeof(double *));
    if (A->values == NULL) {
        fprintf(stderr,
                "Problème lors de l'allocation de l'espace mémoire pour une "
                "matrice : %s\n",
                strerror(errno));

        free_matrix(A);
        exit(EXIT_FAILURE);
    }
    for (uint64_t i = 0; i < m; i++) {
        A->values[i] = (double *)malloc(n * sizeof(double));
        if (A->values[i] == NULL) {
            fprintf(
                stderr,
                "Problème lors de l'allocation de l'espace mémoire pour une "
                "matrice : %s\n",
                strerror(errno));
            free_matrix(A);    
            exit(EXIT_FAILURE);
        }
        for (uint64_t j = 0; j < n; j++) {
            A->values[i][j] = 0;
        }
    }
    return A;
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
    if (A->values == NULL) {
        fprintf(stderr, "Erreur : Mémoire non allouée pour A->values\n");
        exit(EXIT_FAILURE);
    }
    if (B->values == NULL) {
        fprintf(stderr, "Erreur : Mémoire non allouée pour B->values\n");
        exit(EXIT_FAILURE);
    }
    if (C->values == NULL) {
        fprintf(stderr, "Erreur : Mémoire non allouée pour C->values\n");
        exit(EXIT_FAILURE);
    }
    uint64_t m = A->m;
    uint64_t n = A->n;
    if(m != B->m || n != B-> n){
        fprintf(stderr, "Erreur : Dimensions incompatibles A(%" PRIu64 " x %" PRIu64 ") B(%" PRIu64 " x %" PRIu64 ")\n", m, n, B->m, B->n);
;
        exit(EXIT_FAILURE);
    }

    for (uint64_t i = 0; i < m; i++) {
        for (uint64_t j = 0; j < n; j++) {
            C->values[i][j] = A->values[i][j] + B->values[i][j];
        }
    }
}
void sub_m_m(matrix *A, matrix *B, matrix *C) {
    uint64_t m = A->m;
    uint64_t n = A->n;

    if(m != B->m || n != B-> n){
        fprintf(stderr, "Erreur : Dimensions incompatibles A(%" PRIu64 " x %" PRIu64 ") B(%" PRIu64 " x %" PRIu64 ")\n", m, n, B->m, B->n);
        exit(EXIT_FAILURE);
    }

    for (uint64_t i = 0; i < m; i++) {
        for (uint64_t j = 0; j < n; j++) {
            C->values[i][j] = A->values[i][j] - B->values[i][j];
        }
    }
}


void mult_m_v(matrix *A, vector *B, vector *C) {
    uint64_t m = A->m;
    uint64_t n = A->n;
    if(n != B->m){
        fprintf(stderr, "Erreur : Dimensions incompatibles A(%" PRIu64 " x %" PRIu64 ") et B(%" PRIu64 ")\n", m, n, B->m);
        exit(EXIT_FAILURE);
    }
    if(m != C->m){
        fprintf(stderr, "Erreur : Dimensions incompatibles A(%" PRIu64 " x %" PRIu64 ") et C(%" PRIu64 ")\n", m, n, C->m);
        exit(EXIT_FAILURE);
    }
    for (int i = 0; i < m; i++) {
        double res = 0;
        for (int j = 0; j < n; j++) {
            res += A->values[i][j] * B->values[j];
        }
        C->values[i] = res;
    }
}

void mult_m_m(matrix *A, matrix *B, matrix *C){
    uint64_t m = A->m;
    uint64_t n = A->n;
    uint64_t o = B->n;

    if (A->n != B->m) {
        fprintf(stderr, "Erreur: Dimensions incompatibles A(%" PRIu64 " x %" PRIu64 ") et B(%" PRIu64 " x %" PRIu64 ")\n",
                A->m, A->n, B->m, B->n);
        exit(EXIT_FAILURE);
    }

    // Initialisation de la matrice C à 0
    for (uint64_t i = 0; i < m; i++) {
        for (uint64_t j = 0; j < o; j++) {
            C->values[i][j] = 0;
        }
    }

    for (uint64_t i = 0; i < m; i++) {
        for (uint64_t j = 0; j < o; j++) {
            for (uint64_t k = 0; k < n; k++) {
                C->values[i][j] += A->values[i][k] * B->values[k][j];  
            }
        }
    }
}

void transp(matrix*A){
    uint64_t m = A->m;
    uint64_t n = A->n;

    matrix *T = init_matrix(n, m);

    for (uint64_t i = 0; i < m; i++) {
        for (uint64_t j = 0; j < n; j++) {
            T->values[j][i] = A->values[i][j];
        }
    }

    for (uint64_t i = 0; i < m; i++) {
        free(A->values[i]);
    }
    free(A->values);

    A->m = n;
    A->n = m;
    A->values = T->values;

    free(T);

}



void free_matrix(matrix *A){
    if(A != NULL){
        for(uint64_t i=0; i < A->m; i++){
            free(A->values[i]);
        }
        free(A->values);
        free(A);
    }
}

vector *lstsq(matrix *A, vector *b){
    if(A->m < A->n || b->m != A->m){
        fprintf(stderr, "Erreur : dimensions invalides : A(%" PRIu64 " x %" PRIu64 "), b(%" PRIu64 ")",A->m, A->n, b->m);
        exit(EXIT_FAILURE);
    }

    QR_Decomposition *QR = qr(A);
    matrix *Q_t = QR->Q;
    transp(Q_t);
    vector *b_tilde = init_vector(Q_t->m);
    mult_m_v(Q_t, b, b_tilde);
    vector *x = init_vector(b_tilde->m);
    back_sub( b_tilde,QR->R, x);
    return x;
}


void vector_subtract(vector *dest, vector *src, double scalar) {
    for (uint64_t i = 0; i < dest->m; i++) {
        dest->values[i] -= scalar * src->values[i];
    }
}
void vector_divide(vector *v, double scalar) {
    for (uint64_t i = 0; i < v->m; i++) {
        v->values[i] /= scalar;
    }
}




vector* Q_i (matrix *A, uint64_t i) {
    vector *colone = init_vector(A->m);
    if (!colone) return NULL;

    for(uint64_t j = 0; j < A->m; j++){
        colone->values[j] = A->values[j][i];
    }
    return colone;
}

QR_Decomposition *qr(matrix *A) {
    matrix *Q = A;
    matrix *R = init_matrix(A->m, A->n);
    if (!R) return;

    for (uint64_t i = 0; i < A->n; i++) {
        vector *q_i = Q_i(Q, i);
        if (!q_i) return;

        double norm_val = 0;
        norm(q_i, &norm_val);
        R->values[i][i] = norm_val;

        if (norm_val == 0) {
            do {
                for (uint64_t j = 0; j < A->m; j++) {
                    q_i->values[j] = (double)rand() / RAND_MAX;
                }

                for (uint64_t j = 0; j < i; j++) {
                    double dot;
                    vector *q_j = Q_i(Q, j);
                    if (!q_j) return;

                    dot_prod(q_i, q_j, &dot);
                    vector_subtract(q_i, q_j, dot);
                    free(q_j);
                }

                norm(q_i, &norm_val);
            } while (norm_val == 0);
        }

        vector_divide(q_i, norm_val);
        for (uint64_t j = i + 1; j < A->n; j++) {
            vector *q_j = Q_i(Q, j);
            if (!q_j) return;

            dot_prod(q_i, q_j, &R->values[i][j]);
            vector_subtract(q_j, q_i, R->values[i][j]);
            free(q_j);
        }

        free(q_i);
    }

    QR_Decomposition *qr;
    qr->R = R;
    qr->Q = Q;
    return qr;
}




