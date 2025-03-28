#include <CUnit/CUnit.h>
#include <CUnit/Basic.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <inttypes.h>
#include <float.h>
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



void test_sub_v_v(void) {
    uint64_t m = 200;
    vector *v = init_vector(m);
    vector *w = init_vector(m);
    for (uint64_t i = 0; i < m; i++) {
        v->values[i] = (double)rand() / 2.0;
        w->values[i] = (double)rand() / 2.0;
    }
    vector *z = init_vector(m);
    sub_v_v(v, w, z);
    for (uint64_t i = 0; i < m; i++) {
        CU_ASSERT_DOUBLE_EQUAL(z->values[i], v->values[i] - w->values[i], 1e-3);
    }
    free_vector(v);
    free_vector(w);
    free_vector(z);
}

void test_sub_m_m(void) {
    uint64_t m = 100;
    uint64_t n = 50;
    matrix *A = init_matrix(m, n);
    matrix *B = init_matrix(m, n);
    for (uint64_t i = 0; i < m; i++) {
        for (uint64_t j = 0; j < n; j++) {
            A->values[i][j] = (double)rand() / 2.0;
            B->values[i][j] = (double)rand() / 2.0;
        }
    }
    matrix *C = init_matrix(m, n);
    sub_m_m(A, B, C);
    for (uint64_t i = 0; i < m; i++) {
        for (uint64_t j = 0; j < n; j++) {
            CU_ASSERT_DOUBLE_EQUAL(C->values[i][j],
                                   A->values[i][j] - B->values[i][j], 1e-3);
        }
    }
    free_matrix(A);
    free_matrix(B);
    free_matrix(C);
}

<<<<<<< tests/tests_basic_op.c
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
        if (fabs(U->values[i][i]) < DBL_EPSILON) {
            if (fabs(b->values[i]) < DBL_EPSILON) {
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
        if (fabs(U->values[i][i]) < DBL_EPSILON) {
            if (fabs(b->values[i]) < DBL_EPSILON) {
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


void test_dot_prod(void){
    uint64_t m = 100;
    vector *x = init_vector(m);
    vector *y = init_vector(m);

    for (uint64_t i = 0; i < m; i++) {
        x->values[i] = (double)rand() / 2.0; 
        y->values[i] = (double)rand() / 2.0; 
    }

    double result = 0.0;
    dot_prod(x, y, &result);

    double expected_result = 0.0;
    for (uint64_t i = 0; i < m; i++) {
        expected_result += x->values[i] * y->values[i];
    }

    CU_ASSERT_DOUBLE_EQUAL(result, expected_result, 1e-6);

    free_vector(x);
    free_vector(y);
}

void test_mult_m_m(void){
    uint64_t m = 100;
    uint64_t n = 50;
    uint64_t p = 70;
    matrix *A = init_matrix(m, n);
    matrix *B = init_matrix(n, p);
    for (uint64_t i = 0; i < m; i++) {
        for (uint64_t j = 0; j < n; j++) {
            A->values[i][j] = (double)rand() / 2.0;
        }
    }
    for (uint64_t i = 0; i < n; i++) {
        for (uint64_t j = 0; j < p; j++) {
            B->values[i][j] = (double)rand() / 2.0;
        }
    }

    matrix *C = init_matrix(m,p);
    mult_m_m(A, B, C);
    for (uint64_t i = 0; i < m; i++) {
        for (uint64_t j = 0; j < p; j++) {
            double expected = 0;
            for (uint64_t k = 0; k < n; k++) {
                expected += A->values[i][k] * B->values[k][j];
            }
            CU_ASSERT_DOUBLE_EQUAL(C->values[i][j], expected, 1e-6);
        }
    }
    free_matrix(A);
    free_matrix(B);
    free_matrix(C);
}


void test_transp(void){
    uint64_t m = 100;
    uint64_t n = 50;
    matrix *A = init_matrix(m, n);
    matrix *B = init_matrix(n, m);
    for (uint64_t i = 0; i < m; i++) {
        for (uint64_t j = 0; j < n; j++) {
            A->values[i][j] = (double)rand() / 2.0;
            B->values[j][i] = A->values[i][j];
        }
    }
    transp(A);
    for( uint64_t i = 0; i < n; i++){
        for(uint64_t j = 0; j < m; j++){
            CU_ASSERT_DOUBLE_EQUAL(A->values[i][j], B->values[i][j], 1e-3);
        }
    }

    free_matrix(A);
    free_matrix(B);
}


int main(int argc, char **argv) {
    srand(time(NULL));
    if (CUE_SUCCESS != CU_initialize_registry()) {
        return CU_get_error();
    }
    CU_pSuite test_basic_op = CU_add_suite("test_basic_op", NULL, NULL);
    if (test_basic_op == NULL) {
        CU_cleanup_registry();
        return CU_get_error();
    }

    if ((CU_add_test(test_basic_op, "add_v_v", test_add_v_v) == NULL) ||
    (CU_add_test(test_basic_op, "add_m_m", test_add_m_m) == NULL) ||
        (CU_add_test(test_basic_op, "add_m_m", test_add_m_m) == NULL) ||
        (CU_add_test(test_basic_op, "norm", test_norm) == NULL)|| 
        (CU_add_test(test_basic_op, "sub_m_m", test_sub_m_m) == NULL) ||
        (CU_add_test(test_basic_op, "sub_v_v",test_sub_v_v) == NULL) || 
        (CU_add_test(test_basic_op, "dot_prod",test_dot_prod) == NULL) ||
        (CU_add_test(test_basic_op, "mult_m_m", test_mult_m_m) == NULL) ||
        (CU_add_test(test_basic_op, "mult_m_v", test_mult_m_v) == NULL) ||
        (CU_add_test(test_basic_op, "transp", test_transp) == NULL))
    (CU_add_test(test_basic_op, "back_sub_identity_matrix", test_back_sub_identity_matrix) == NULL) ||
    (CU_add_test(test_basic_op, "back_sub_diagonal_matrix", test_back_sub_diagonal_matrix) == NULL) ||
    (CU_add_test(test_basic_op, "back_sub_upper_triangular", test_back_sub_upper_triangular) == NULL) ||
    (CU_add_test(test_basic_op, "back_sub_zero_diagonal", test_back_sub_zero_diagonal) == NULL) ||
    (CU_add_test(test_basic_op, "back_sub_zero_diagonal", test_back_sub_zero_diagonal_inf) == NULL) ||
    (CU_add_test(test_basic_op, "back_sub_single_element", test_back_sub_single_element) == NULL) || 
    (CU_add_test(test_basic_op, "back_sub_4x4_upper_triangular", test_back_sub_4x4_upper_triangular) == NULL)||
    (CU_add_test(test_basic_op, "back_sub_90x90_upper_triangular", test_back_sub_90x90_upper_triangular) == NULL)) {
    CU_cleanup_registry();
    return CU_get_error();
    }
    CU_basic_set_mode(CU_BRM_VERBOSE);
    CU_basic_run_tests();
    
    CU_cleanup_registry();
    return 0;
}
``
