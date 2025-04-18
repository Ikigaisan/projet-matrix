#include "../headers/matrix.h"
#include "../headers/vector.h"
#include <float.h>
#include <math.h>
#include <inttypes.h>
#include <stdbool.h>

/* 
 * Allocation et initialisation d'une matrice de dimensions m x n.
 * La matrice est initialisée avec des zéros.
 */
matrix *init_matrix(uint64_t m, uint64_t n) {
    matrix *A = (matrix *) malloc(sizeof(matrix));
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
    // Allocation et initialisation à zéro de chaque ligne
    for (uint64_t i = 0; i < m; i++) {
        A->values[i] = (double *)malloc(n * sizeof(double));
        if (A->values[i] == NULL) {
            fprintf(
                stderr,
                "Problème lors de l'allocation de l'espace mémoire pour une "
                "matrice : %s\n",
                strerror(errno));
            exit(EXIT_FAILURE);
        }
        for (uint64_t j = 0; j < n; j++) {
            A->values[i][j] = 0;
        }
    }
    return A;
}

/* 
 * Libération de la mémoire allouée pour une matrice.
 */
void free_matrix(matrix *A){
    if(A != NULL){
        for(uint64_t i = 0; i < A->m; i++){
            free(A->values[i]);
        }
        free(A->values);
        free(A);
    }
}

/* 
 * Affichage de la matrice au format lisible.
 */
void print_matrix(matrix *A) {
    if (A->m == 0) {
        printf("[[]]\n");
        return;
    }
    // Affichage de la première ligne de la matrice
    printf("[[%f", A->values[0][0]);
    for (uint64_t j = 1; j < A->n; j++) {
        printf(" %f", A->values[0][j]);
    }
    if (A->m == 1) {
        printf("]]\n");
        return;
    }
    printf("]\n");
    // Affichage des lignes suivantes
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

/* 
 * Addition de deux matrices A et B et stockage du résultat dans C.
 * Les dimensions de A, B et C doivent être compatibles.
 */
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
        exit(EXIT_FAILURE);
    }
    // Addition élément par élément
    for (uint64_t i = 0; i < m; i++) {
        for (uint64_t j = 0; j < n; j++) {
            C->values[i][j] = A->values[i][j] + B->values[i][j];
        }
    }
}

/* 
 * Soustraction de deux matrices A et B et stockage du résultat dans C.
 */
void sub_m_m(matrix *A, matrix *B, matrix *C) {
    uint64_t m = A->m;
    uint64_t n = A->n;
    if(m != B->m || n != B-> n){
        fprintf(stderr, "Erreur : Dimensions incompatibles A(%" PRIu64 " x %" PRIu64 ") B(%" PRIu64 " x %" PRIu64 ")\n", m, n, B->m, B->n);
        exit(EXIT_FAILURE);
    }
    // Soustraction élément par élément
    for (uint64_t i = 0; i < m; i++) {
        for (uint64_t j = 0; j < n; j++) {
            C->values[i][j] = A->values[i][j] - B->values[i][j];
        }
    }
}

/* 
 * Multiplication d'une matrice A par un vecteur B, résultat dans C.
 */
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
    // Calcul du produit matrice-vecteur
    for (int i = 0; i < m; i++) {
        double res = 0;
        for (int j = 0; j < n; j++) {
            res += A->values[i][j] * B->values[j];
        }
        C->values[i] = res;
    }
}

/* 
 * Multiplication de deux matrices A et B, résultat dans C.
 */
void mult_m_m(matrix *A, matrix *B, matrix *C){
    uint64_t m = A->m;
    uint64_t n = A->n;
    uint64_t o = B->n;
    if (A->n != B->m) {
        fprintf(stderr, "Erreur: Dimensions incompatibles A(%" PRIu64 " x %" PRIu64 ") et B(%" PRIu64 " x %" PRIu64 ")\n",
                A->m, A->n, B->m, B->n);
        exit(EXIT_FAILURE);
    }
    // Initialisation de C à zéro
    for (uint64_t i = 0; i < m; i++) {
        for (uint64_t j = 0; j < o; j++) {
            C->values[i][j] = 0;
        }
    }
    // Calcul standard de la multiplication matricielle
    for (uint64_t i = 0; i < m; i++) {
        for (uint64_t j = 0; j < o; j++) {
            for (uint64_t k = 0; k < n; k++) {
                C->values[i][j] += A->values[i][k] * B->values[k][j];  
            }
        }
    }
}

/* 
 * Transposition d'une matrice A.
 * La matrice transposée remplace l'originale.
 */
void transp(matrix*A){
    uint64_t m = A->m;
    uint64_t n = A->n;
    // Création d'une matrice temporaire T pour stocker la transposée
    matrix *T = init_matrix(n, m);
    for (uint64_t i = 0; i < m; i++) {
        for (uint64_t j = 0; j < n; j++) {
            T->values[j][i] = A->values[i][j];
        }
    }
    // Libération des anciennes lignes de A
    for (uint64_t i = 0; i < m; i++) {
        free(A->values[i]);
    }
    free(A->values);
    // Mise à jour des dimensions et du tableau de valeurs de A
    A->m = n;
    A->n = m;
    A->values = T->values;
    free(T);
}

/* 
 * Résolution par substitution arrière.
 * Résout un système triangulaire supérieur Ux = b.
 */
void back_sub(vector*b, matrix *U, vector*x){
    uint64_t m = b->m;
    bool infinite = false;
    // Copie initiale de b dans x
    for (uint64_t i = 0; i < m; i++ ){
        x->values[i] = b->values[i];
    }
    // Cas particulier : une seule inconnue
    if (m == 1) {
        if (fabs(U->values[0][0]) < DBL_EPSILON) {
            if (fabs(b->values[0]) < DBL_EPSILON) {
                // Infinité de solutions, on attribue une valeur arbitraire
                x->values[0] = 1.0;
                fprintf(stderr, "Infinité de solutions\n");
            } else {
                fprintf(stderr, "Aucune solution\n");
                return;
            }
        } else {
            x->values[0] = b->values[0] / U->values[0][0];
        }
        return;
    }
    if (m < 1){
        fprintf(stderr, "La matrice n'est pas valide.\n");
        return;
    }
    // Résolution par substitution arrière pour m > 1
    for (int64_t i = m-1; i >= 0; i--){
        for (int64_t j = m-1; j > i; j--){
            x->values[i] -= U->values[i][j] * x->values[j]; 
        }
        if (fabs(U->values[i][i]) < DBL_EPSILON){
            if (fabs(x->values[i]) < DBL_EPSILON){
                x->values[i] = 1; // Attribution arbitraire en cas d'infinité de solutions
                infinite = true;
            }
            else {
                fprintf(stderr, "Aucune solution\n");
                return;
            }
        }
        x->values[i] /= U->values[i][i];
    }
    if (infinite == true){
        fprintf(stderr, "Infinité de solutions\n");
    }
}
void free_qr(QR_Decomposition *qr) {
    if (qr != NULL) {
        if (qr->Q != NULL)
            free_matrix(qr->Q);
        if (qr->R != NULL)
            free_matrix(qr->R);
        free(qr);
    }
}

/* 
 * Résolution des moindres carrés.
 * Trouve la meilleure approximation x pour A*x ≈ b via une décomposition QR.
 */
vector *lstsq(matrix *A, vector *b){
    if(A->m < A->n || b->m != A->m){
        fprintf(stderr, "Erreur : dimensions invalides : A(%" PRIu64 " x %" PRIu64 "), b(%" PRIu64 ")", A->m, A->n, b->m);
        exit(EXIT_FAILURE);
    }
    QR_Decomposition *QR = qr(A);
    matrix *Q_t = QR->Q;
    transp(Q_t);
    vector *b_tilde = init_vector(Q_t->m);
    mult_m_v(Q_t, b, b_tilde);
    vector *x = init_vector(b_tilde->m);
    back_sub( b_tilde,QR->R, x);
    free_vector(b_tilde);
    free_qr(QR);
    return x;
}

/* 
 * Soustraction d'un vecteur src multiplié par un scalaire au vecteur dest.
 */
void vector_subtract(vector *dest, vector *src, double scalar) {
    for (uint64_t i = 0; i < dest->m; i++) {
        dest->values[i] -= scalar * src->values[i];
    }
}

/* 
 * Division de chaque élément d'un vecteur par un scalaire.
 */
void vector_divide(vector *v, double scalar) {
    for (uint64_t i = 0; i < v->m; i++) {
        v->values[i] /= scalar;
    }
}

/* 
 * Extrait la ième colonne d'une matrice A sous forme de vecteur.
 */
vector* Q_i (matrix *A, uint64_t i) {
    vector *colone = init_vector(A->m);
    if (!colone) return NULL;
    for(uint64_t j = 0; j < A->m; j++){
        colone_i->values[j] = A->values[j][i];
    }
    return colone_i;
}


QR_Decomposition *qr(matrix *A) {
    uint64_t m = A->m, n = A->n;

    // Allouer les matrices Q et R
    matrix *Q = init_matrix(m, n);
    matrix *R = init_matrix(n, n);

    if (!Q || !R) {
        fprintf(stderr, "Erreur d'allocation mémoire pour Q ou R.\n");
        free_matrix(Q);
        free_matrix(R);
        return NULL;
    }

    // Initialiser les valeurs de Q et R
    for (uint64_t i = 0; i < m; i++) {
        for (uint64_t j = 0; j < n; j++) {
            Q->values[i][j] = A->values[i][j];
        }
    }

    for (uint64_t i = 0; i < n; i++) {
        for (uint64_t j = 0; j < n; j++) {
            R->values[i][j] = 0.0;
        }
    }

    // Decomposition QR
    for (uint64_t i = 0; i < n; i++) {
        vector *q_i = Q_i(Q, i);
        if (!q_i) {
            free_matrix(Q);
            free_matrix(R);
            return NULL;
        }

        double norm_val = 0;
        norm(q_i, &norm_val);
        R->values[i][i] = norm_val;

        // cas spécifique si R (i,i) = 0
        while (fabs(norm_val) < DBL_EPSILON) {
            for (uint64_t j = 0; j < A->m; j++) {
                q_i->values[j] = (double)rand() / RAND_MAX;
            }

            for (uint64_t j = 0; j < i; j++) {
                double dot;
                vector *q_j = Q_i(Q, j);
                if (!q_j) {
                    free_vector(q_i);
                    free_matrix(Q);
                    free_matrix(R);
                    return NULL;
                }

                dot_prod(q_i, q_j, &dot);
                vector_subtract(q_i, q_j, dot);
                free_vector(q_j);  // Libérer q_j après utilisation
            }

            norm(q_i, &norm_val);
        }

        // Normalisation de q_i
        for (uint64_t k = 0; k < m; k++) {
            Q->values[k][i] /= norm_val;
        }

        // Calcul de R et mise à jour de Q
        for (uint64_t j = i + 1; j < n; j++) {
            double dot = 0.0;
            for (uint64_t k = 0; k < m; k++) {
                dot += Q->values[k][i] * Q->values[k][j];
            }
            R->values[i][j] = dot;

            for (uint64_t k = 0; k < m; k++) {
                Q->values[k][j] -= dot * Q->values[k][i];
            }
        }

        // Libérer q_i après utilisation
        free_vector(q_i);
    }

    // Allouer la structure QR_Decomposition
    QR_Decomposition *qr = malloc(sizeof(QR_Decomposition));
    if (!qr) {
        fprintf(stderr, "Erreur d'allocation mémoire pour QR_Decomposition.\n");
        free_matrix(Q);
        free_matrix(R);
        return NULL;
    }

    qr->R = R;
    qr->Q = Q;

    return qr;
}

