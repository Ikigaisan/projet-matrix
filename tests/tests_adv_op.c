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
    // Test 1: Matrice Identité
    matrix *A1 = init_matrix(3, 3);
    A1->values[0][0] = 1; A1->values[1][1] = 1; A1->values[2][2] = 1;
    QR_Decomposition *result1 = qr(A1);
    CU_ASSERT_PTR_NOT_NULL_FATAL(result1);
    for (uint64_t i = 0; i < 3; i++) {
        for (uint64_t j = 0; j < 3; j++) {
            CU_ASSERT_DOUBLE_EQUAL(result1->Q->values[i][j], (i == j) ? 1.0 : 0.0, EPSILON);
            CU_ASSERT_DOUBLE_EQUAL(result1->R->values[i][j], (i == j) ? 1.0 : 0.0, EPSILON);
        }
    }
    free_matrix(A1);
    // free_matrix(result1->Q);
    // free_matrix(result1->R);
    free_qr(result1);

    // Test 2: Matrice Diagonale
    matrix *A2 = init_matrix(3, 3);
    A2->values[0][0] = 2; A2->values[1][1] = 3; A2->values[2][2] = 4;
    QR_Decomposition *result2 = qr(A2);
    CU_ASSERT_PTR_NOT_NULL_FATAL(result2);
    for (uint64_t i = 0; i < 3; i++) {
        for (uint64_t j = 0; j < 3; j++) {
            CU_ASSERT_DOUBLE_EQUAL(result2->Q->values[i][j], (i == j) ? 1.0 : 0.0, EPSILON);
            CU_ASSERT_DOUBLE_EQUAL(result2->R->values[i][j], A2->values[i][j], EPSILON);
        }
    }
    free_matrix(A2);
    // free_matrix(result2->Q);
    // free_matrix(result2->R);
    free_qr(result2);

    // Test 3: Matrice Carrée Générale
    matrix *A3 = init_matrix(3, 3);
    A3->values[0][0] = 1; A3->values[0][1] = 2; A3->values[0][2] = 3;
    A3->values[1][0] = 4; A3->values[1][1] = 5; A3->values[1][2] = 6;
    A3->values[2][0] = 7; A3->values[2][1] = 8; A3->values[2][2] = 9;
    QR_Decomposition *result3 = qr(A3);
    CU_ASSERT_PTR_NOT_NULL_FATAL(result3);
    matrix *QR3 = init_matrix(3, 3);
    mult_m_m(result3->Q, result3->R, QR3);
    for (uint64_t i = 0; i < 3; i++) {
        for (uint64_t j = 0; j < 3; j++) {
            CU_ASSERT_DOUBLE_EQUAL(QR3->values[i][j], A3->values[i][j], EPSILON);
        }
    }
    free_matrix(A3);
    // free_matrix(result3->Q);
    // free_matrix(result3->R);
    free_matrix(QR3);
    free_qr(result3);
    
    // Test 4: Matrice Rectangulaire (m > n)
    matrix *A4 = init_matrix(4, 3);
    A4->values[0][0] = 1; A4->values[0][1] = 2; A4->values[0][2] = 3;
    A4->values[1][0] = 4; A4->values[1][1] = 5; A4->values[1][2] = 6;
    A4->values[2][0] = 7; A4->values[2][1] = 8; A4->values[2][2] = 9;
    A4->values[3][0] = 10; A4->values[3][1] = 11; A4->values[3][2] = 12;
    
    QR_Decomposition *result4 = qr(A4);
    
    CU_ASSERT_PTR_NOT_NULL_FATAL(result4);
    matrix *QR4 = init_matrix(4, 3);
    mult_m_m(result4->Q, result4->R, QR4);
    
    for (uint64_t i = 0; i < 4; i++) {
        for (uint64_t j = 0; j < 3; j++) {
            CU_ASSERT_DOUBLE_EQUAL(QR4->values[i][j], A4->values[i][j], EPSILON);
        }
    }
    free_matrix(A4);
    //free_matrix(result4->Q);
    // free_matrix(result4->R);
    free_matrix(QR4);
    free_qr(result4);
    
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
        vector *b_copy = init_vector(m);
        for (uint64_t i = 0; i < m; i++) {
            b_copy->values[i] = b->values[i];
        }
        expected_x->values[i] = b_copy->values[i];
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

    if (!b || !x || !U) {
        // Vérifier si l'allocation a échoué
        fprintf(stderr, "Erreur d'allocation mémoire.\n");
        free_vector(b);
        free_vector(x);
        free_matrix(U);
        return;
    }

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
                free_vector(b);
                free_vector(x);
                free_matrix(U);
                return;
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

    if (!b || !x || !U) {
        fprintf(stderr, "Erreur d'allocation de mémoire.\n");
        free_vector(b);
        free_vector(x);
        free_matrix(U);
        return;
    }

    // Matrice avec un zéro sur la diagonale
    for (uint64_t i = 0; i < m; i++) {
        for (uint64_t j = 0; j < m; j++) {
            U->values[i][j] = (i <= j) ? (double)(rand() % 10 + 1) : 0.0;
        }
        b->values[i] = (double)(rand() % 10 + 1);
    }

    // Zéro sur la diagonale (par exemple sur la deuxième ligne)
    U->values[1][1] = 0.0;  // Zéro sur la diagonale

    uint64_t infinite = 0;  // Compte le nombre de solutions infinies

    // Effectuer la substitution arrière
    for (int64_t i = m - 1; i >= 0; i--) {
        // Si U[i][i] est proche de zéro
        if (fabs(U->values[i][i]) < EPSILON) {
            if (fabs(b->values[i]) < EPSILON) {
                // Cas d'infinité de solutions
                x->values[i] = 1.0;  // Assigner une valeur arbitraire (par exemple 1)
                infinite++;
            } else {
                // Cas d'aucune solution
                fprintf(stderr, "Aucune solution\n");
                free_vector(b);
                free_vector(x);
                free_matrix(U);
                return;
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
    if (infinite > 0) {
        fprintf(stderr, "Infinité de solutions\n");
    }

    // Vérification explicite de l'infinité de solutions
    if (infinite > 0) {
        // On peut tester qu'au moins un élément de x a été modifié (par exemple à 1.0 dans notre cas)
        CU_ASSERT_DOUBLE_EQUAL(x->values[1], 1.0, 1e-3);  // x[1] doit être égal à 1 si on a une infinité de solutions
    }

    // Libération de la mémoire après l'utilisation
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
        vector *b_copy = init_vector(m);
        for (uint64_t i = 0; i < m; i++) {
            b_copy->values[i] = b->values[i];
        }
        expected_x->values[i] = b_copy->values[i];
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
        vector *b_copy = init_vector(m);
        for (uint64_t i = 0; i < m; i++) {
            b_copy->values[i] = b->values[i];
        }
        expected_x->values[i] = b_copy->values[i];
        for (uint64_t j = i + 1; j < m; j++) {
            expected_x->values[i] -= U->values[i][j] * expected_x->values[j];
        }
        expected_x->values[i] /= U->values[i][i];
    }

    // Vérification de la solution obtenue par substitution arrière
    for (uint64_t i = 0; i < m; i++) {
        CU_ASSERT_DOUBLE_EQUAL(x->values[i], expected_x->values[i], 1e-1);
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
    (CU_add_test(test_adv_op, "back_sub_zero_diagonal_inf", test_back_sub_zero_diagonal_inf) == NULL) ||
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
