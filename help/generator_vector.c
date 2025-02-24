#include <endian.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include "../headers/file.h"
#include "../headers/vector.h"

int main(int argc, char **argv) {
    // Modify here
    uint64_t m = 3;
    vector *v = init_vector(m);
    v->values[0] = -2.0;
    v->values[1] = 3.4;
    v->values[2] = 1.7;
    char *file_name = "vector_A.bin";
    // Stop modify

    FILE *file = fopen(file_name, "w+");
    write_vector(v, file);
    free(v);
    fclose(file);
}