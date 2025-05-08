#include <endian.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>

#include "../headers/file.h"
#include "../headers/matrix.h"

void make_file_matrix(char* filename){
    uint64_t m = 3;
    uint64_t n = 3;
    matrix *A = init_matrix(m, n);
    for(uint64_t i = 0; i<m; i++){
        for(uint64_t j = 0; j<n; j++){
            A->values[i][j] = rand()/2;
        }
    }
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