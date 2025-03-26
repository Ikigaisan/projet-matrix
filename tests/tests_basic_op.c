#include <CUnit/Basic.h>
#include <CUnit/CUnit.h>
#include <time.h>

#include "../headers/matrix.h"
#include "../headers/vector.h"

#include "../headers/file.h"

void test_add_v_v(void) {
    uint64_t m = 300;
    vector *v = init_vector(m);
    vector *w = init_vector(m);
    for (uint64_t i = 0; i < m; i++) {
        v->values[i] = (double)rand() / 2.0;
        w->values[i] = (double)rand() / 2.0;
    }
    vector *z = init_vector(m);
    add_v_v(v, w, z);
    for (uint64_t i = 0; i < m; i++) {
        CU_ASSERT_DOUBLE_EQUAL(z->values[i], v->values[i] + w->values[i], 1e-3);
    }
    free(v);
    free(w);
    free(z);
}

void test_add_m_m(void) {
    uint64_t m = 300;
    uint64_t n = 200;
    matrix *A = init_matrix(m, n);
    matrix *B = init_matrix(m, n);
    for (uint64_t i = 0; i < m; i++) {
        for (uint64_t j = 0; j < n; j++) {
            A->values[i][j] = (double)rand() / 2.0;
            B->values[i][j] = (double)rand() / 2.0;
        }
    }
    matrix *C = init_matrix(m, n);
    add_m_m(A, B, C);
    for (uint64_t i = 0; i < m; i++) {
        for (uint64_t j = 0; j < n; j++) {
            CU_ASSERT_DOUBLE_EQUAL(C->values[i][j],
                                   A->values[i][j] + B->values[i][j], 1e-3);
        }
    }
    free(A);
    free(B);
    free(C);
}

void test_back_sub(void) {
    uint64_t m = 3;
    vector *b = init_vector(m);
    vector *x = init_vector(m);
    matrix *U = init_matrix(m, m);

    // Initialisation de la matrice triangulaire supérieure U
    for (uint64_t i = 0; i < m; i++) {
        for (uint64_t j = 0; j < m; j++) {
            U->values[i][j] = (i <= j) ? (double)(rand() % 10 + 1) : 0.0;
        }
    }

    // Initialisation du vecteur b
    for (uint64_t i = 0; i < m; i++) {
        b->values[i] = (double)(rand() % 10 + 1);
    }

    // Exécution de la substitution arrière
    back_sub(b, U, x);

    // Vérification des résultats avec une tolérance
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

    // Libération de la mémoire
    free(b);
    free(x);
    free(U);
    free(expected_x);
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
        (CU_add_test(test_basic_op, "add_m_m", test_add_m_m) == NULL)) {
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