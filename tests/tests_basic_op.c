#include <CUnit/CUnit.h>
#include <CUnit/Basic.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <inttypes.h>

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
    
    double expected_norm = sqrt(3.0 * 3.0 + 4.0 * 4.0);
    double computed_norm;
    norm(v, &computed_norm);
    
    CU_ASSERT_DOUBLE_EQUAL(computed_norm, expected_norm, 1e-3);
    
    free_vector(v);
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
        (CU_add_test(test_basic_op, "norm", test_norm) == NULL)|| 
        (CU_add_test(test_basic_op, "sub_m_m", test_sub_m_m) == NULL) ||
        (CU_add_test(test_basic_op, "sub_v_v",test_sub_v_v) == NULL) || 
        (CU_add_test(test_basic_op, "dot_prod",test_dot_prod) == NULL) ||
        (CU_add_test(test_basic_op, "mult_m_m", test_mult_m_m) == NULL) ||
        (CU_add_test(test_basic_op, "mult_m_v", test_mult_m_v) == NULL) ||
        (CU_add_test(test_basic_op, "transp", test_transp) == NULL)){
        CU_cleanup_registry();
        return CU_get_error();
    }
    CU_basic_set_mode(CU_BRM_VERBOSE);
    CU_basic_run_tests();
    
    CU_cleanup_registry();
    return 0;
}
``
