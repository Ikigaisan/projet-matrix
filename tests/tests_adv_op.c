#include <CUnit/Basic.h>
#include <CUnit/CUnit.h>
#include <time.h>
#include <math.h>
#include <inttypes.h>

#include "../headers/matrix.h"
#include "../headers/vector.h"
#include "../headers/file.h"

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


int main(int argc, char **argv) {
    srand(time(NULL));
    if (CUE_SUCCESS != CU_initialize_registry()) {
        return CU_get_error();
    }
    CU_pSuite test_basic_op = CU_add_suite("Test_Adv_Op_Suite", NULL, NULL);
    if (test_basic_op == NULL) {
        CU_cleanup_registry();
        return CU_get_error();
    }
    if (CU_add_test(test_basic_op, "test_lstsq", test_lstsq) == NULL){
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
