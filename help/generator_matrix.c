#include <endian.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include "../headers/file.h"
#include "../headers/matrix.h"

void make_file_matrix(char* filename){
    uint64_t m = 3;
    uint64_t n = 1;
    matrix *A = init_matrix(m, n);
    A->values[0][0] = rand()/2;
    A->values[1][0] = rand()/2;
    A->values[2][0] = rand()/2;
    char *file_name = filename;
    FILE *file = fopen(file_name, "w+");
    write_matrix(A, file);
    print_matrix(A);
    free(A);
    fclose(file);
}

int main(int argc, char **argv) {
    printf("Matrix A :\n");
    make_file_matrix("matrix_A.bin");
    printf("Matrix B :\n");
    make_file_matrix("matrix_B.bin");
}