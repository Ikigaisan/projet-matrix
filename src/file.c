#include "../headers/file.h"
#include "../headers/matrix.h"
#include "../headers/vector.h"
#include <inttypes.h>


void write_double(double value,FILE *file) {
    if(file == NULL){
        fprintf(stderr,"Erreur : fichier non valide.\n");
        exit(EXIT_FAILURE);
    }

    size_t result = fwrite(&value, sizeof(double), 1, file);
    if(result != 1){
        fprintf(stderr, "Erreur lors de l'écriture dans le fichier.\n");
        fclose(file);
        exit(EXIT_FAILURE);
    }
}

vector *read_vector(FILE *file){
    uint64_t size;
    if(fread(&size, sizeof(uint64_t), 1, file) != 1){
            fprintf(stderr, "Erreur lors de la récupération de la tailledu vecteur.\n");
            exit(EXIT_FAILURE);
    }
    
    size = be64toh(size);

    vector *result = init_vector(size);
    if(fread(result->values, sizeof(double),size,file) != size){
        fprintf(stderr,"Erreur lors de la lecture des valeurs du vecteur dans le fichier.\n");
        free_vector(result);
        fclose(file);
        exit(EXIT_FAILURE);
    }

    return result;

}

void write_vector(vector *v, FILE *file){

    if(!v || !file){
        fprintf(stderr, "Erreur : Pointeur null passé en paramètre.");
        if(v) free_vector(v);
        if(file) fclose(file);
        exit(EXIT_FAILURE);
    }

    uint64_t size = htobe64(v->m);
    if(fwrite(&size, sizeof(uint64_t), 1, file) != 1){
        fprintf(stderr, "Erreur lors de l'écriture dans le fichier. \n");
        free_vector(v);
        fclose(file);
        exit(EXIT_FAILURE);
    }

    if(fwrite(v->values, sizeof(double), v->m, file) != v->m){
        fprintf(stderr, "Erreur lors de l'écriture dans lefichier. \n");
        free_vector(v);
        fclose(file);
        exit(EXIT_FAILURE);
    }
}

matrix *read_matrix(FILE *file) {
    if(!file){
        fprintf(stderr,"Erreur : Pointeur null passé en paramètre.\n");
        exit(EXIT_FAILURE);
    }

    uint64_t rows, cols;

    // Lire les dimensions de la matrice
    if(fread(&rows, sizeof(uint64_t), 1, file) != 1 ||
    fread(&cols, sizeof(uint64_t), 1, file) != 1){
        fprintf(stderr, "Erreur lors de la lecture des dimensions de la matrice.\n");
        fclose(file);
        exit(EXIT_FAILURE);
    }

    rows = be64toh(rows);
    cols = be64toh(cols);

    // Afficher les dimensions lues
    printf("Dimensions de la matrice lues : rows = %" PRIu64 ", cols = %" PRIu64 "\n", rows, cols);
    fflush(stdout);

    matrix *result = init_matrix(rows, cols);

    if (!result) {
        fprintf(stderr, "Erreur d'allocation de la matrice.\n");
        fclose(file);
        exit(EXIT_FAILURE);
    }

    // Lire les valeurs de la matrice
    for(uint64_t i = 0; i < rows; i++){
        if(fread(result->values[i], sizeof(double), cols, file) != cols){
            fprintf(stderr, "Erreur lors de la lecture d'une ligne.\n");
            free_matrix(result);
            fclose(file);
            exit(EXIT_FAILURE);
        }
    }

    return result;
}


void write_matrix(matrix *A, FILE* file){
    if(!file || !A){
        fprintf(stderr,"Erreur : Pointeur null passé en paramètre.\n");
        if(A) free_matrix(A);
        if(file) fclose(file);
        exit(EXIT_FAILURE);
    }

    uint64_t m_A = A->m;
    uint64_t n_A = A->n;

    uint64_t m_A_be = htobe64(m_A);
    uint64_t n_A_be = htobe64(n_A);

    if(fwrite(&m_A_be, sizeof(uint64_t), 1, file) != 1 ||
    fwrite(&n_A_be,sizeof(uint64_t), 1, file) != 1){
        fprintf(stderr, "Erreur lors de l'écritude des dimensions dans le fichier.");
        fclose(file);
        exit(EXIT_FAILURE);
    }

    for(uint64_t i = 0; i< m_A; i++){
        uint64_t line_nbr = htobe64(i);
        if(fwrite(&line_nbr, sizeof(uint64_t), 1, file) != 1){
            fprintf(stderr, "Erreur lors de l'écriture du numéro de la ligne.");
            free_matrix(A);
            fclose(file);
            exit(EXIT_FAILURE);
        }
        for(uint64_t j = 0; j< n_A; j++){
            if(fwrite(&(A->values[i][j]),sizeof(double), 1, file) != 1){
                fprintf(stderr, "Erreur lors de l'écriture d'une valeur.");
                free_matrix(A);
                fclose(file);
                exit(EXIT_FAILURE);
            }
        }
    }
}

QR_Decomposition *read_QR(FILE *file) {
    printf("Allocation de la mémoire pour la décomposition QR \n");
    fflush(stdout);
    QR_Decomposition *qr = (QR_Decomposition *) malloc(sizeof(QR_Decomposition));
    if (!qr) {
        fprintf(stderr, "Erreur d'allocation mémoire pour QR_Decomposition.\n");
        exit(EXIT_FAILURE);  // Utiliser exit ou retourner NULL pour signaler l'erreur
    }

    // Vérifier que le fichier est valide
    if (!file) {
        fprintf(stderr, "Erreur : fichier invalide.\n");
        free(qr);  // Libérer la mémoire allouée avant de quitter
        exit(EXIT_FAILURE);
    }
    printf("Allocation de la mémoire pour Q \n");
    fflush(stdout);
    qr->Q = (matrix *) malloc(sizeof(matrix));
    printf("Allocation de la mémoire pour R \n");
    fflush(stdout);
    qr->R = (matrix *) malloc(sizeof(matrix));

    if (!qr->Q || !qr->R) {
        fprintf(stderr, "Erreur d'allocation mémoire pour les matrices Q et R.\n");
        if(qr->Q) free(qr->Q);
        if(qr->R) free(qr->R);
        free(qr);
        exit(EXIT_FAILURE);
    }
    // Lire la matrice Q et R depuis le fichier
    printf("Premier read matrix \n");
    fflush(stdout);
    qr->Q = read_matrix(file);  // Lire la matrice Q
    if (!qr->Q) {
        fprintf(stderr, "Erreur lors de la lecture de la matrice Q.\n");
        free(qr);  // Libérer la mémoire allouée pour QR_Decomposition
        exit(EXIT_FAILURE);
    }

    printf("Deuxième read matrix \n");
    fflush(stdout);
    qr->R = read_matrix(file);  // Lire la matrice R
    if (!qr->R) {
        fprintf(stderr, "Erreur lors de la lecture de la matrice R.\n");
        free_matrix(qr->Q);  // Libérer la mémoire de Q si R échoue
        free(qr);
        exit(EXIT_FAILURE);
    }

    // Retourner la structure contenant les matrices Q et R
    return qr;
}



void write_QR(matrix *Q, matrix *R, FILE *file){
    write_matrix(Q, file);
    write_matrix(R, file);
}