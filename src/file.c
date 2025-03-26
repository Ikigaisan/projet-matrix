#include "../headers/file.h"
#include "../headers/matrix.h"
#include "../headers/vector.h"


void write_double(double value,FILE *file) {
    if(file == NULL){
        fprintf(stderr,"Erreur : fichier non valide.\n");
        exit(EXIT_FAILURE);
    }

    size_t result = fwrite(&value, sizeof(double), 1, file);
    if(result != 1){
        fprintf(stderr, "Erreur lors de l'écriture dans le fichier.\n");
    }
}

vector *read_vector(FILE *file){
    uint64_t size;
    if(fread(&size, sizeof(uint64_t), 1, file) != 1){
            fprintf(stderr, "Erreur lors de la récupération de la tailledu vecteur.\n");
            exit(EXIT_FAILURE);
    }

    vector *result = init_vector(size);
    if(fread(result->values, sizeof(double),size,file) != size){
        fprintf(stderr,"Erreur lors de la lecture des valeurs du vecteur dans le fichier.\n");
        free_vector(result);
        exit(EXIT_FAILURE);
    }

    return result;

}

void write_vector(vector *v, FILE *file){

    if(!v || !file){
        fprintf(stderr, "Erreur : Pointeur null passé en paramètre.");
        exit(EXIT_FAILURE);
    }

    if(fwrite(&(v->m), sizeof(uint64_t), 1, file) != 1){
        fprintf(stderr, "Erreur lors de l'écriture dans le fichier. \n");
        exit(EXIT_FAILURE);
    }

    if(fwrite(v->values, sizeof(double), v->m, file) != 1){
        fprintf(stderr, "Erreur lors de l'écriture dans lefichier. \n");
        exit(EXIT_FAILURE);
    }
}

matrix *read_matrix(FILE *file){
    if(!file){
        fprintf(stderr,"Erreur : Pointeur null passé en paramètre.\n");
        exit(EXIT_FAILURE);
    }

    uint64_t rows, cols;

    if(fread(&rows, sizeof(uint64_t), 1, file) != 1 ||
    fread(&cols, sizeof(uint64_t), 1, file) != 1){
        fprintf(stderr, "Erreur lors de la lecture des dimensions de la matrice\n.");
        exit(EXIT_FAILURE);
    }
    
    matrix *result = init_matrix(rows, cols);

    for(uint64_t i = 0; i<rows; i++){
        if(fread(result->values[i],sizeof(double), cols, file) != cols){
            fprintf(stderr, "Erreur lors de la lecture d'une ligne.\n");
            free_matrix(result);
            exit(EXIT_FAILURE);
        }
        
    }
    return result;

}