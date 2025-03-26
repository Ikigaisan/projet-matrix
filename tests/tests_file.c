#include <CUnit/Basic.h>
#include <CUnit/CUnit.h>
#include <time.h>
#include <math.h>

#include "../headers/matrix.h"
#include "../headers/vector.h"
#include "../headers/file.h"

void test_write_double(void) {
    FILE *file = fopen("test.bin", "wb");
    CU_ASSERT_PTR_NOT_NULL(file);
    
    double value = 3.14;
    write_double(value, file);
    
    fclose(file);
    
    // Vérification du fichier
    file = fopen("test.bin", "rb");
    CU_ASSERT_PTR_NOT_NULL(file);
    
    double read_value;
    fread(&read_value, sizeof(double), 1, file);
    CU_ASSERT_DOUBLE_EQUAL(value, read_value, 0.0001);
    
    fclose(file);
}

void test_read_vector(void) {
    FILE *file = fopen("vector.bin", "wb");
    CU_ASSERT_PTR_NOT_NULL(file);

    uint64_t size = 3;
    vector *v = init_vector(size);
    v->values[0] = 1.0;
    v->values[1] = 2.0;
    v->values[2] = 3.0;
    
    write_vector(v, file);
    fclose(file);

    file = fopen("vector.bin", "rb");
    vector *v_read = read_vector(file);
    CU_ASSERT_EQUAL(v_read->m, size);
    CU_ASSERT_DOUBLE_EQUAL(v_read->values[0], 1.0, 0.0001);
    CU_ASSERT_DOUBLE_EQUAL(v_read->values[1], 2.0, 0.0001);
    CU_ASSERT_DOUBLE_EQUAL(v_read->values[2], 3.0, 0.0001);

    fclose(file);
    free_vector(v);
    free_vector(v_read);
}

void test_write_vector(){
    FILE *file = fopen("vector.bin", "wb");
    CU_ASSERT_PTR_NOT_NULL(file);

    vector *a = init_vector((uint64_t) 100);

    for(uint64_t i = 0; i<100; i++){
        a->values[i] = (double) rand()/2;
    }
    write_vector(a, file);
    fclose(file);

    file = fopen("vector.bin", "rb");
    CU_ASSERT_PTR_NOT_NULL(file);

    vector *v_read = read_vector(file);

    for(uint64_t i =0 ; i<100; i++){
        CU_ASSERT_DOUBLE_EQUAL(v_read->values[i], a->values[i], 1e-3);
    }
    fclose(file);

}

int main() {
    CU_initialize_registry();

    CU_pSuite suite = CU_add_suite("File_Test_Suite", 0, 0);
    if(CU_add_test(suite, "test_write_double", test_write_double) == NULL ||
    CU_add_test(suite, "test_write_vector", test_write_vector) == NULL){
        CU_cleanup_registry();
        return CU_get_error();
    }

    CU_basic_run_tests();
    CU_cleanup_registry();
    return 0;
}