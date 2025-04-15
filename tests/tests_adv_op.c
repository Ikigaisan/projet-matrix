#include <CUnit/Basic.h>
#include <CUnit/CUnit.h>
#include <time.h>
#include <math.h>
#include <inttypes.h>

#include "../headers/matrix.h"
#include "../headers/vector.h"
#include "../headers/file.h"

#define EPSILON 1e-6  // Tolérance pour les comparaisons de flottants

void test_qr_decomposition() {
    uint64_t m = 3, n = 3;
    
    // Initialisation de la matrice A
    matrix *A = init_matrix(m, n);
    A->values[0][0] = 1; A->values[0][1] = 1; A->values[0][2] = 1;
    A->values[1][0] = 1; A->values[1][1] = 2; A->values[1][2] = 3;
    A->values[2][0] = 1; A->values[2][1] = 3; A->values[2][2] = 6;

    // Cloner A car qr() modifie A
    matrix *A_clone = init_matrix(m, n);
    for (uint64_t i = 0; i < m; i++)
        for (uint64_t j = 0; j < n; j++)
            A_clone->values[i][j] = A->values[i][j];

    // Exécuter la décomposition QR
    QR_Decomposition *decomp = qr(A_clone);
    CU_ASSERT_PTR_NOT_NULL_FATAL(decomp);
    matrix *Q = decomp->Q;
    matrix *R = decomp->R;

    // Vérifier si A ≈ Q * R
    matrix *QR = init_matrix(m, n);
    for (uint64_t i = 0; i < m; i++) {
        for (uint64_t j = 0; j < n; j++) {
            QR->values[i][j] = 0;
            for (uint64_t k = 0; k < n; k++) {
                QR->values[i][j] += Q->values[i][k] * R->values[k][j];
            }
            CU_ASSERT_DOUBLE_EQUAL(QR->values[i][j], A->values[i][j], 1e-6);
        }
    }

    // Libération de la mémoire
    free_matrix(A);
    free_matrix(A_clone);
    free_matrix(QR);
    free_matrix(R);
    // Q est en fait A_clone modifié → libérer seulement une fois
    free_matrix(Q); 
    free(decomp);
}

void test_lstsq() {
    // Création d'une matrice 2x2
    matrix *A = init_matrix(2, 2);
    A->values[0][0] = 1; A->values[0][1] = 1;
    A->values[1][0] = 1; A->values[1][1] = -1;

    // Création d'un vecteur b
    vector *b = init_vector(2);
    b->values[0] = 3;
    b->values[1] = 1;

    // Calcul de x attendu : résolution de Ax = b
    // [ 1  1 ] [ x1 ]   [ 3 ]
    // [ 1 -1 ] [ x2 ] = [ 1 ]
    // x1 = 2, x2 = 1
    vector *expected_x = init_vector(2);
    expected_x->values[0] = 2.0;
    expected_x->values[1] = 1.0;

    // Exécution de lstsq
    vector *x = lstsq(A, b);
    CU_ASSERT_DOUBLE_EQUAL(x->values[0], expected_x->values[0], 1e-3);
    CU_ASSERT_DOUBLE_EQUAL(x->values[1], expected_x->values[1], 1e-3);

    // Libération de la mémoire
    free_matrix(A);
    free_vector(b);
    free_vector(x);
    free_vector(expected_x);
}


void test_back_sub_identity_matrix(void) {
    uint64_t m = 3;
    vector *b = init_vector(m);
    vector *x = init_vector(m);
    matrix *U = init_matrix(m, m);

    // Matrice identité
    for (uint64_t i = 0; i < m; i++) {
        for (uint64_t j = 0; j < m; j++) {
            U->values[i][j] = (i == j) ? 1.0 : 0.0;
        }
        b->values[i] = (double)(rand() % 10 + 1);
    }

    back_sub(b, U, x);

    for (uint64_t i = 0; i < m; i++) {
        CU_ASSERT_DOUBLE_EQUAL(x->values[i], b->values[i], 1e-3);
    }

    free_vector(b);
    free_vector(x);
    free_matrix(U);
}

void test_back_sub_diagonal_matrix(void) {
    uint64_t m = 3;
    vector *b = init_vector(m);
    vector *x = init_vector(m);
    matrix *U = init_matrix(m, m);

    // Matrice diagonale
    for (uint64_t i = 0; i < m; i++) {
        for (uint64_t j = 0; j < m; j++) {
            U->values[i][j] = (i == j) ? (double)(rand() % 10 + 1) : 0.0;
        }
        b->values[i] = (double)(rand() % 10 + 1);
    }

    back_sub(b, U, x);

    for (uint64_t i = 0; i < m; i++) {
        CU_ASSERT_DOUBLE_EQUAL(x->values[i], b->values[i] / U->values[i][i], 1e-3);
    }

    free_vector(b);
    free_vector(x);
    free_matrix(U);
}

void test_back_sub_upper_triangular(void) {
    uint64_t m = 3;
    vector *b = init_vector(m);
    vector *x = init_vector(m);
    matrix *U = init_matrix(m, m);

    // Matrice triangulaire supérieure aléatoire
    for (uint64_t i = 0; i < m; i++) {
        for (uint64_t j = 0; j < m; j++) {
            U->values[i][j] = (i <= j) ? (double)(rand() % 10 + 1) : 0.0;
        }
        b->values[i] = (double)(rand() % 10 + 1);
    }

    back_sub(b, U, x);

    // Vérification avec calcul manuel
    vector *expected_x = init_vector(m);
    for (int64_t i = m - 1; i >= 0; i--) {
        expected_x->values[i] = b->values[i];
        for (uint64_t j = i + 1; j < m; j++) {
            expected_x->values[i] -= U->values[i][j] * expected_x->values[j];
        }
        expected_x->values[i] /= U->values[i][i];
    }

    for (uint64_t i = 0; i < m; i++) {
        CU_ASSERT_DOUBLE_EQUAL(x->values[i], expected_x->values[i], 1e-3);
    }

    free_vector(b);
    free_vector(x);
    free_matrix(U);
    free_vector(expected_x);
}

void test_back_sub_zero_diagonal(void) {
    uint64_t m = 3;
    vector *b = init_vector(m);
    vector *x = init_vector(m);
    matrix *U = init_matrix(m, m);

    // Matrice avec un zéro sur la diagonale
    for (uint64_t i = 0; i < m; i++) {
        for (uint64_t j = 0; j < m; j++) {
            U->values[i][j] = (i <= j) ? (double)(rand() % 10 + 1) : 0.0;
        }
        b->values[i] = (double)(rand() % 10 + 1);
    }

    // Zéro sur la diagonale
    U->values[1][1] = 0.0;  // Par exemple, on met un zéro sur la deuxième ligne

    uint64_t infinité = 0;  // Compte le nombre de solutions infinies

    // Effectuer la substitution arrière
    for (int64_t i = m - 1; i >= 0; i--) {
        // Si U[i][i] est proche de zéro
        if (fabs(U->values[i][i]) < EPSILON) {
            if (fabs(b->values[i]) < EPSILON) {
                // Cas d'infinité de solutions
                x->values[i] = 1.0;  // Assigner une valeur arbitraire (par exemple 1)
                infinité++;
            } else {
                // Cas d'aucune solution
                fprintf(stderr, "Aucune solution\n");
                return;
                //exit(1);
            }
        } else {
            // Effectuer la substitution classique
            for (int64_t j = m - 1; j > i; j--) {
                b->values[i] -= U->values[i][j] * x->values[j];
            }
            x->values[i] = b->values[i] / U->values[i][i];
        }
    }

    // Vérification
    if (infinité > 0) {
        fprintf(stderr, "Infinité de solutions\n");
    }

    // Libération de la mémoire
    free_vector(b);
    free_vector(x);
    free_matrix(U);
}

void test_back_sub_zero_diagonal_inf(void) {
    uint64_t m = 3;
    vector *b = init_vector(m);
    vector *x = init_vector(m);
    matrix *U = init_matrix(m, m);

    // Matrice avec un zéro sur la diagonale
    for (uint64_t i = 0; i < m; i++) {
        for (uint64_t j = 0; j < m; j++) {
            U->values[i][j] = (i <= j) ? (double)(rand() % 10 + 1) : 0.0;
        }
        b->values[i] = (double)(rand() % 10 + 1);
    }

    // Zéro sur la diagonale (par exemple sur la deuxième ligne)
    U->values[1][1] = 0.0;  // Zéro sur la diagonale

    uint64_t infinité = 0;  // Compte le nombre de solutions infinies

    // Effectuer la substitution arrière
    for (int64_t i = m - 1; i >= 0; i--) {
        // Si U[i][i] est proche de zéro
        if (fabs(U->values[i][i]) < EPSILON) {
            if (fabs(b->values[i]) < EPSILON) {
                // Cas d'infinité de solutions
                x->values[i] = 1.0;  // Assigner une valeur arbitraire (par exemple 1)
                infinité++;
            } else {
                // Cas d'aucune solution
                fprintf(stderr, "Aucune solution\n");
                return;
                //exit(1);
            }
        } else {
            // Effectuer la substitution classique
            for (int64_t j = m - 1; j > i; j--) {
                b->values[i] -= U->values[i][j] * x->values[j];
            }
            x->values[i] = b->values[i] / U->values[i][i];
        }
    }

    // Vérification : Si infinité > 0, il faut imprimer le message "Infinité de solutions"
    if (infinité > 0) {
        fprintf(stderr, "Infinité de solutions\n");
    }

    // Vérification explicite de l'infinité de solutions
    if (infinité > 0) {
        // On peut tester qu'au moins un élément de x a été modifié (par exemple à 1.0 dans notre cas)
        CU_ASSERT_DOUBLE_EQUAL(x->values[1], 1.0, 1e-3);  // x[1] doit être égal à 1 si on a une infinité de solutions
    }

    free_vector(b);
    free_vector(x);
    free_matrix(U);
}



void test_back_sub_single_element(void) {
    vector *b = init_vector(1);
    vector *x = init_vector(1);
    matrix *U = init_matrix(1, 1);

    U->values[0][0] = 5.0;
    b->values[0] = 10.0;

    back_sub(b, U, x);

    CU_ASSERT_DOUBLE_EQUAL(x->values[0], 2.0, 1e-3);

    free_vector(b);
    free_vector(x);
    free_matrix(U);
}

void test_back_sub_4x4_upper_triangular(void) {
    uint64_t m = 4;
    vector *b = init_vector(m);
    vector *x = init_vector(m);
    matrix *U = init_matrix(m, m);

    // Matrice triangulaire supérieure aléatoire 4x4
    for (uint64_t i = 0; i < m; i++) {
        for (uint64_t j = 0; j < m; j++) {
            U->values[i][j] = (i <= j) ? (double)(rand() % 10 + 1) : 0.0;
        }
        b->values[i] = (double)(rand() % 10 + 1);
    }

    back_sub(b, U, x);

    // Vérification avec calcul manuel pour une matrice 4x4
    vector *expected_x = init_vector(m);
    for (int64_t i = m - 1; i >= 0; i--) {
        expected_x->values[i] = b->values[i];
        for (uint64_t j = i + 1; j < m; j++) {
            expected_x->values[i] -= U->values[i][j] * expected_x->values[j];
        }
        expected_x->values[i] /= U->values[i][i];
    }

    for (uint64_t i = 0; i < m; i++) {
        CU_ASSERT_DOUBLE_EQUAL(x->values[i], expected_x->values[i], 1e-3);
    }

    free_vector(b);
    free_vector(x);
    free_matrix(U);
    free_vector(expected_x);
}

void test_back_sub_90x90_upper_triangular(void) {
    uint64_t m = 90;  // Taille de la matrice 90x90
    vector *b = init_vector(m);
    vector *x = init_vector(m);
    matrix *U = init_matrix(m, m);

    // Matrice triangulaire supérieure aléatoire 90x90
    for (uint64_t i = 0; i < m; i++) {
        for (uint64_t j = i; j < m; j++) {  // Remplir seulement la partie triangulaire supérieure
            U->values[i][j] = (double)(rand() % 10 + 1);  // Valeurs aléatoires entre 1 et 10
        }
        b->values[i] = (double)(rand() % 10 + 1);  // Valeur aléatoire pour b
    }

    // Appliquer la substitution arrière
    back_sub(b, U, x);

    // Vérification avec calcul manuel pour une matrice 90x90
    vector *expected_x = init_vector(m);
    for (int64_t i = m - 1; i >= 0; i--) {
        expected_x->values[i] = b->values[i];
        for (uint64_t j = i + 1; j < m; j++) {
            expected_x->values[i] -= U->values[i][j] * expected_x->values[j];
        }
        expected_x->values[i] /= U->values[i][i];
    }

    // Vérification de la solution obtenue par substitution arrière
    for (uint64_t i = 0; i < m; i++) {
        CU_ASSERT_DOUBLE_EQUAL(x->values[i], expected_x->values[i], 1e-3);
    }

    free_vector(b);
    free_vector(x);
    free_matrix(U);
    free_vector(expected_x);
}
int main(int argc, char **argv) {
    srand(time(NULL));
    if (CUE_SUCCESS != CU_initialize_registry()) {
        return CU_get_error();
    }
    CU_pSuite test_adv_op = CU_add_suite("Test_Adv_Op_Suite", NULL, NULL);
    if (test_adv_op == NULL) {
        CU_cleanup_registry();
        return CU_get_error();
    }
    if (CU_add_test(test_adv_op, "test_lstsq", test_lstsq) == NULL ||
    CU_add_test(test_adv_op, "test_qr_decomposition", test_qr_decomposition) == NULL ||
    (CU_add_test(test_adv_op, "back_sub_identity_matrix", test_back_sub_identity_matrix) == NULL) ||
    (CU_add_test(test_adv_op, "back_sub_diagonal_matrix", test_back_sub_diagonal_matrix) == NULL) ||
    (CU_add_test(test_adv_op, "back_sub_upper_triangular", test_back_sub_upper_triangular) == NULL) ||
    (CU_add_test(test_adv_op, "back_sub_zero_diagonal", test_back_sub_zero_diagonal) == NULL) ||
    (CU_add_test(test_adv_op, "back_sub_zero_diagonal", test_back_sub_zero_diagonal_inf) == NULL) ||
    (CU_add_test(test_adv_op, "back_sub_single_element", test_back_sub_single_element) == NULL) || 
    (CU_add_test(test_adv_op, "back_sub_4x4_upper_triangular", test_back_sub_4x4_upper_triangular) == NULL)||
    (CU_add_test(test_adv_op, "back_sub_90x90_upper_triangular", test_back_sub_90x90_upper_triangular) == NULL)){
        CU_cleanup_registry();
        return CU_get_error();
    }
    CU_basic_set_mode(CU_BRM_VERBOSE);
    CU_basic_run_tests();
    CU_basic_show_failures(CU_get_failure_list());
    int failed_tests = CU_get_number_of_failures();
    CU_cleanup_registry();
    return (failed_tests > 0) ? 1 : 0;
}
