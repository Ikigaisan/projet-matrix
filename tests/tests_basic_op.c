#include <CUnit/CUnit.h>
#include <CUnit/Basic.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "matrix.h"  // Assurez-vous d'inclure le fichier contenant init_matrix et qr_decomposition

#define EPSILON 1e-6  // Tolérance pour les comparaisons de flottants

void test_qr_decomposition() {
    uint64_t m = 3, n = 3;
    
    // Initialisation de la matrice A
    matrix *A = init_matrix(m, n);
    A->values[0][0] = 1; A->values[0][1] = 1; A->values[0][2] = 1;
    A->values[1][0] = 1; A->values[1][1] = 2; A->values[1][2] = 3;
    A->values[2][0] = 1; A->values[2][1] = 3; A->values[2][2] = 6;

    // Matrices Q et R
    matrix *Q = init_matrix(m, n);
    matrix *R = init_matrix(n, n);

    // Exécuter la décomposition QR
    qr_decomposition(A, Q, R);

    // Vérifier si A ≈ Q * R
    matrix *QR = init_matrix(m, n);
    for (uint64_t i = 0; i < m; i++) {
        for (uint64_t j = 0; j < n; j++) {
            QR->values[i][j] = 0;
            for (uint64_t k = 0; k < n; k++) {
                QR->values[i][j] += Q->values[i][k] * R->values[k][j];
            }
            // Vérification avec une tolérance
            CU_ASSERT(fabs(A->values[i][j] - QR->values[i][j]) < EPSILON);
        }
    }

    // Libération de la mémoire
    for (uint64_t i = 0; i < m; i++) {
        free(A->values[i]);
        free(Q->values[i]);
        free(R->values[i]);
        free(QR->values[i]);
    }
    free(A->values); free(Q->values); free(R->values); free(QR->values);
    free(A); free(Q); free(R); free(QR);
}

int main() {
    CU_initialize_registry();
    
    CU_pSuite suite = CU_add_suite("QR Decomposition Test Suite", 0, 0);
    CU_add_test(suite, "test_qr_decomposition", test_qr_decomposition);
    
    CU_basic_set_mode(CU_BRM_VERBOSE);
    CU_basic_run_tests();
    
    CU_cleanup_registry();
    return 0;
}
``
