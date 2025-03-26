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

int main() {
    CU_initialize_registry();

    CU_pSuite suite = CU_add_suite("File_Test_Suite", 0, 0);
    CU_add_test(suite, "test_write_double", test_write_double);

    CU_basic_run_tests();
    CU_cleanup_registry();
    return 0;
}