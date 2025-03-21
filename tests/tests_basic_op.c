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
    free_vector(v);
    free_vector(w);
    free_vector(z);
}

void test_add_m_m(void) {
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
    add_m_m(A, B, C);
    for (uint64_t i = 0; i < m; i++) {
        for (uint64_t j = 0; j < n; j++) {
            CU_ASSERT_DOUBLE_EQUAL(C->values[i][j],
                                   A->values[i][j] + B->values[i][j], 1e-3);
        }
    }
    free_matrix(A);
    free_matrix(B);
    free_matrix(C);
}


void test_mult_m_v(void) {
    uint64_t m = 100;
    uint64_t n = 50;
    matrix *A = init_matrix(m, n);
    vector *B = init_vector(n);
    for (uint64_t i = 0; i < m; i++) {
        for (uint64_t j = 0; j < n; j++) {
            A->values[i][j] = (double)rand() / 2.0;
        }
    }
    for (uint64_t i = 0; i < n; i++) {
        B->values[i] = (double)rand() / 2.0;
    }
    vector *C = init_vector(m);
    mult_m_v(A, B, C);
    for (uint64_t i = 0; i < m; i++) {
        double res = 0;
        for (uint64_t j = 0; j < n; j++) {
            res += A->values[i][j] * B->values[j];
        }
        CU_ASSERT_DOUBLE_EQUAL(C->values[i], res, 1e-3);
    }
    free_matrix(A);
    free_vector(B);
    free_vector(C);

}

void test_norm(void) {
    uint64_t m = 5;
    vector *v = init_vector(m);
    
    double values[] = {3.0, 4.0, 0.0, 0.0, 0.0};
    for (uint64_t i = 0; i < m; i++) {
        v->values[i] = values[i];
    }
    
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
    vector *x = init_vector(3);
    vector *y = init_vector(3);
    x->values[0] = 1;
    x-> values[1] = 2;
    x->values[2] = 3;
    y->values[0] = 1;
    y-> values[1] = 3;
    y->values[2] = 4;

    print_vector(x);
    print_vector(y);

    double result = 0.0;
    dot_prod(x, y, &result);
    CU_ASSERT_EQUAL(result, 19);


    for(u_int64_t i = 0; i<3; i++){
        x->values[i] = 0;
        y->values[i] = 0;
    }
    dot_prod(x, x, &result);
    CU_ASSERT_EQUAL(result, 0);

    free_vector(x);
    free_vector(y);

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
        (CU_add_test(test_basic_op, "norm", test_norm) == NULL)|| CU_add_test(test_basic_op, "sub_m_m", test_sub_m_m) == NULL ||
        (CU_add_test(test_basic_op, "sub_v_v",test_sub_v_v) == NULL)|| (CU_add_test(test_basic_op, "dot_prod",test_dot_prod) == NULL)){
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
