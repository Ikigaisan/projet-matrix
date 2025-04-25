#include <stdio.h>
#include <stdlib.h>
#include "../headers/error.h"

void handle_error(int code) {
    switch (code) {
        case ERROR_NULL_POINTER:
            fprintf(stderr, "Erreur : pointeur NULL détecté.\n");
            break;
        case ERROR_NULL_VALUES:
            fprintf(stderr, "Erreur : valeurs NULL détecté.\n");
            break;
        case ERROR_SIZE_MISMATCH:
            fprintf(stderr, "Erreur : dimensions incompatibles.\n");
            break;
        case ERROR_ALLOC:
            fprintf(stderr, "Erreur : échec de l’allocation mémoire.\n");
            break;
        default:
            fprintf(stderr, "Erreur inconnue (code %d).\n", code);
    }
    exit(EXIT_FAILURE);
}
