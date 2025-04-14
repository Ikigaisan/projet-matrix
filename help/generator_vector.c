#include <endian.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include "../headers/file.h"
#include "../headers/vector.h"

void make_file_vector(char *filename){
    uint64_t m = 3;
    vector *v = init_vector(m);
    v->values[0] = rand()/2;
    v->values[1] =rand()/2;
    v->values[2] = rand()/2;
    char *file_name = filename ;

    FILE *file = fopen(file_name, "w+");
    write_vector(v, file);
    print_vector(v);
    free(v);
    fclose(file);
}

int main(int argc, char **argv) {
    printf("Vector A :\n");
    make_file_vector("vector_A.bin");
    printf("Vector B :\n");
    make_file_vector("vector_B.bin");
}