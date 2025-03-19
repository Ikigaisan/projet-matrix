#include <CUnit/Basic.h>
#include <CUnit/CUnit.h>
#include <time.h>
#include <math.h>

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

void test_norm(void) {
    uint64_t m = 5;
    vector *v = init_vector(m);
    vector *w = init_vector(m);
    
    double values1[] = {1.0, 2.0, 3.0, 4.0, 5.0};
    double values2[] = {5.0, 4.0, 3.0, 2.0, 1.0};
    
    for (uint64_t i = 0; i < m; i++) {
        v->values[i] = values1[i];
        w->values[i] = values2[i];
    }
    
    uint64_t expected_norm = (uint64_t)sqrt(16 + 4 + 0 + 4 + 16);
    uint64_t computed_norm;
    norm(v, w, &computed_norm);
    
    CU_ASSERT_EQUAL(computed_norm, expected_norm);
    
    free(v);
    free(w);
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
        (CU_add_test(test_basic_op, "norm", test_norm) == NULL)) {
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
