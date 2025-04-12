#include <CUnit/Basic.h>
#include <CUnit/CUnit.h>
#include <time.h>
#include <math.h>
#include <inttypes.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <unistd.h>

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
    int size = 50;
    matrix *A = init_matrix(size, size);

    for(uint64_t i = 0; i < size; i++){
        for(uint64_t j = 0; j < size; j++){
            A->values[i][j] = (double)rand() / 2;
        }
    }

    write_matrix(A, file);
    fclose(file);

    file = fopen("matrix.bin", "rb");
    CU_ASSERT_PTR_NOT_NULL(file);

    matrix *M_read = read_matrix(file);

    for(uint64_t i = 0; i < size; i++){
        for(uint64_t j = 0; j < size; j++){
            CU_ASSERT_DOUBLE_EQUAL(M_read->values[i][j], A->values[i][j], 1e-3);
        }
    }

    free_matrix(A);
    free_matrix(M_read);
    fclose(file);
}


void test_write_read_QR() {
    
    FILE *file = fopen("QR.bin", "wb");
    CU_ASSERT_PTR_NOT_NULL(file);
    uint64_t m = 3;
    uint64_t n = 2;

    matrix *Q = init_matrix(m, n);
    matrix *R = init_matrix(n, n);


    for (uint64_t i = 0; i < m; i++) {
        for (uint64_t j = 0; j < n; j++) {
            Q->values[i][j] = (double)rand() / 2;
        }
    }

    for (uint64_t i = 0; i < n; i++) {
        for (uint64_t j = 0; j < n; j++) {
            R->values[i][j] = (double)rand() / 2;
        }
    }

    printf("Matrice Q :\n");
    print_matrix(Q);
    printf("Matrice R :\n");
    print_matrix(R);
    printf("Debut de write QR\n");
    write_QR(Q, R, file);
    printf("fin de write QR\n");
    
    fclose(file);

    file = fopen("QR.bin", "rb");
    CU_ASSERT_PTR_NOT_NULL(file);

    printf("Debut de read QR\n");
    QR_Decomposition *read_qr = read_QR(file);
    printf("fin de read QR\n");
    CU_ASSERT_PTR_NOT_NULL(read_qr);

    

    for(uint64_t i = 0; i<m; i++){
        for(uint64_t j = 0; j<n; j++){
            CU_ASSERT_DOUBLE_EQUAL(read_qr->Q->values[i][j], Q->values[i][j], 1e-3);
        }
    }

    for(uint64_t i = 0; i<n; i++){
        for(uint64_t j = 0; j<n; j++){
            CU_ASSERT_DOUBLE_EQUAL(read_qr->R->values[i][j], R->values[i][j], 1e-3);
        }
    }

    fclose(file);

    free_matrix(read_qr->Q);
    free_matrix(read_qr->R);
    free_matrix(Q);
    free_matrix(R);

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


    CU_basic_set_mode(CU_BRM_VERBOSE);
    CU_basic_run_tests();
    CU_basic_show_failures(CU_get_failure_list());
    int failed_tests = CU_get_number_of_failures();
    CU_cleanup_registry();
    return (failed_tests > 0) ? 1 : 0;

    return 0;
}
