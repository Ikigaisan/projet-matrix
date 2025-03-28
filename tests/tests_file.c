#include <CUnit/Basic.h>
#include <CUnit/CUnit.h>
#include <time.h>
#include <math.h>
#include <inttypes.h>
#include "../headers/matrix.h"
#include "../headers/vector.h"
#include "../headers/file.h"

void test_write_double(void) {
    FILE *file = fopen("double.bin", "wb");
    CU_ASSERT_PTR_NOT_NULL(file);
    
    double value = 3.14;
    write_double(value, file);
    
    fclose(file);
    
    file = fopen("double.bin", "rb");
    CU_ASSERT_PTR_NOT_NULL(file);
    
    double read_value;
    fread(&read_value, sizeof(double), 1, file);
    CU_ASSERT_DOUBLE_EQUAL(value, read_value, 0.0001);
    
    fclose(file);
}

void test_write_read_vector(void) {
    FILE *file = fopen("vector.bin", "wb");
    CU_ASSERT_PTR_NOT_NULL(file);

    uint64_t size = 50;
    vector *v = init_vector(size);
    for(uint64_t i = 0; i< size; i++){
        v->values[i] = (double) rand()/2;
    }
    
    write_vector(v, file);
    fclose(file);

    file = fopen("vector.bin", "rb");
    vector *v_read = read_vector(file);
    CU_ASSERT_EQUAL(v_read->m, size);
    for (uint64_t i = 0; i< size ; i++){
        CU_ASSERT_DOUBLE_EQUAL(v_read->values[i], v->values[i], 1e-3);
    }

    fclose(file);
    free_vector(v);
    free_vector(v_read);
}

void test_write_read_matrix(void) {
    FILE *file = fopen("matrix.bin", "wb");
    CU_ASSERT_PTR_NOT_NULL(file);

    matrix *A = init_matrix(100, 100);

    for(uint64_t i = 0; i < 100; i++){
        for(uint64_t j = 0; j < 100; j++){
            A->values[i][j] = (double)rand() / 2;
        }
    }

    write_matrix(A, file);
    fclose(file);

    file = fopen("matrix.bin", "rb");
    CU_ASSERT_PTR_NOT_NULL(file);

    matrix *M_read = read_matrix(file);

    for(uint64_t i = 0; i < 100; i++){
        for(uint64_t j = 0; j < 100; j++){
            CU_ASSERT_DOUBLE_EQUAL(M_read->values[i][j], A->values[i][j], 1e-3);
        }
    }

    free_matrix(A);
    free_matrix(M_read);
    fclose(file);
}

void test_write_read_QR(void) {
    FILE *file = fopen("QR.bin", "wb");
    CU_ASSERT_PTR_NOT_NULL(file);

    matrix *Q = init_matrix(100, 100);
    matrix *R = init_matrix(100, 100);

    // Remplir Q et R avec des valeurs aléatoires
    for(uint64_t i = 0; i < 100; i++) {
        for(uint64_t j = 0; j < 100; j++) {
            Q->values[i][j] = (double)rand() / 2;
            R->values[i][j] = (double)rand() / 2;
        }
    }

    // Écrire Q et R dans le fichier
    write_QR(Q, R, file);
    fclose(file);

    // Lire les matrices Q et R depuis le fichier
    file = fopen("QR.bin", "rb");
    CU_ASSERT_PTR_NOT_NULL(file);

    matrix *Q_read = init_matrix(100, 100);
    matrix *R_read = init_matrix(100, 100);
    read_QR(file, Q_read, R_read);

    // Vérifier les valeurs lues
    for(uint64_t i = 0; i < 100; i++) {
        for(uint64_t j = 0; j < 100; j++) {
            CU_ASSERT_DOUBLE_EQUAL(Q_read->values[i][j], Q->values[i][j], 1e-3);
            CU_ASSERT_DOUBLE_EQUAL(R_read->values[i][j], R->values[i][j], 1e-3);
        }
    }

    free_matrix(Q);
    free_matrix(R);
    free_matrix(Q_read);
    free_matrix(R_read);
    fclose(file);
}

int main() {
    CU_initialize_registry();

    CU_pSuite suite = CU_add_suite("File_Test_Suite", 0, 0);
    if (CU_add_test(suite, "test_write_double", test_write_double) == NULL ||
        CU_add_test(suite, "test_write_read_vector", test_write_read_vector) == NULL ||
        CU_add_test(suite, "test_write_read_matrix", test_write_read_matrix) == NULL ||
        CU_add_test(suite, "test_write_read_QR", test_write_read_QR) == NULL) {
        CU_cleanup_registry();
        return CU_get_error();
    }

    CU_basic_run_tests();
    CU_cleanup_registry();

    return 0;
}
