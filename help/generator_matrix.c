#include <endian.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include "../headers/file.h"
#include "../headers/matrix.h"

int main(int argc, char** argv) {
    // Modify here
    uint64_t m = 3;
    uint64_t n = 1;
    matrix* A = init_matrix(m,n);
    A->values[0][0] = -2.0;
    A->values[1][0] = 3.4;
    A->values[2][0] = 1.7;
    char* file_name = "matrix_A.bin";
    // Stop modify

    FILE* file = fopen(file_name, "w+");
    write_matrix(A, file); free(A);
    fclose(file);
}