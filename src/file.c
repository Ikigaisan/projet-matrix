#include "../headers/file.h"
#include "../headers/matrix.h"
#include "../headers/vector.h"
#include <inttypes.h>
#include <stdio.h>

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

    // Ecriture de la taille du vecteur
    uint64_t size = htobe64(v->m);
    if(fwrite(&size, sizeof(uint64_t), 1, file) != 1){
        fprintf(stderr, "Erreur lors de l'écriture dans le fichier. \n");
        free_vector(v);
        fclose(file);
        exit(EXIT_FAILURE);
    }

    // Ecriture du vecteur
    if(fwrite(v->values, sizeof(double), v->m, file) != v->m){
        fprintf(stderr, "Erreur lors de l'écriture dans le fichier. \n");
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

    // Lecture du nombre de colonnes et de lignes de la matrice
    if(fread(&rows, sizeof(uint64_t), 1, file) != 1 ||
    fread(&cols, sizeof(uint64_t), 1, file) != 1){
        fprintf(stderr, "Erreur lors de la lecture des dimensions de la matrice.\n");
        fclose(file);
        exit(EXIT_FAILURE);
    }

    rows = be64toh(rows);
    cols = be64toh(cols);

    fprintf(stderr,"Lecture matrice de dimensions: %" PRIu64 " x %" PRIu64 "\n", rows, cols);

    matrix *result = init_matrix(rows, cols);
    

    // Lecture de chaque ligne qu'on stocke dans result
    for(uint64_t i = 0; i < rows; i++){
        uint64_t line_nbr;
        if(fread(&line_nbr, sizeof(uint64_t),1, file) !=1){
            fprintf(stderr, "Erreur lors de la lecture du numéro de la ligne\n");
            free_matrix(result);
            fclose(file);
            exit(EXIT_FAILURE);
        }
        line_nbr = be64toh(line_nbr);
        if(line_nbr != i){
            fprintf(stderr, "Mauvaise ligne\n");
            free_matrix(result);
            fclose(file);
            exit(EXIT_FAILURE);
        }
        if(fread(result->values[i], sizeof(double), cols, file) != cols){
            fprintf(stderr, "Erreur lors de la lecture de la ligne.\n");
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
    // Pour les boucles
    uint64_t m_A = A->m;
    uint64_t n_A = A->n;

    // Pour écrire dans le fichier
    uint64_t m_A_be = htobe64(m_A);
    uint64_t n_A_be = htobe64(n_A);

    // Ecriture du nombre de lignes et colonnes
    if(fwrite(&m_A_be, sizeof(uint64_t), 1, file) != 1 ||
    fwrite(&n_A_be,sizeof(uint64_t), 1, file) != 1){
        fprintf(stderr, "Erreur lors de l'écriture des dimensions dans le fichier.");
        fclose(file);
        exit(EXIT_FAILURE);
    }

    // Ecriture de chaque ligne
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
    if(!file){
        fprintf(stderr, "Erreur : file est un pointeur null.\n");
        exit(EXIT_FAILURE);
    }

    QR_Decomposition *qr = (QR_Decomposition *) malloc(sizeof(QR_Decomposition));
    if (!qr) {
        fprintf(stderr, "Erreur d'allocation mémoire pour QR_Decomposition.\n");
        exit(EXIT_FAILURE);
    }
    
    uint64_t m, n;

    // Lecture des dimensions de la matrice Q
    if (fread(&m, sizeof(uint64_t), 1, file) != 1 || fread(&n, sizeof(uint64_t), 1, file) != 1) {
        fprintf(stderr, "Erreur lors de la lecture des dimensions de Q.\n");
        free(qr);
        fclose(file);
        exit(EXIT_FAILURE);
    }
    
    m = be64toh(m);
    n = be64toh(n);

    // Lecture de la matrice Q
    qr->Q = init_matrix(m, n);
    for (uint64_t i = 0; i < m; i++) {
        if (fread(qr->Q->values[i], sizeof(double), n, file) != n) {
            fprintf(stderr, "Erreur lors de la lecture des valeurs de la matrice Q.\n");
            free_matrix(qr->Q);
            free(qr);
            fclose(file);
            exit(EXIT_FAILURE);
        }
    }

    // Lecture de la matrice R
    qr->R = init_matrix(n, n);
    for (uint64_t i = 0; i < n; i++) {
        if (fread(qr->R->values[i], sizeof(double), n, file) != n) {
            fprintf(stderr, "Erreur lors de la lecture des valeurs de la matrice R.\n");
            free_matrix(qr->Q);
            free_matrix(qr->R);
            free(qr);
            fclose(file);
            exit(EXIT_FAILURE);
        }
    }

    return qr;
}



void write_QR(matrix *Q, matrix *R, FILE *file){

    if (!file || !Q || !R) {
        fprintf(stderr, "Erreur : Pointeur null passé en paramètre.\n");
        exit(EXIT_FAILURE);
    }

    uint64_t m = htobe64(Q->m);
    uint64_t n = htobe64(Q->n);

    // Ecriture des dimensions
    if (fwrite(&m, sizeof(uint64_t), 1, file) != 1 || fwrite(&n, sizeof(uint64_t), 1, file) != 1) {
        fprintf(stderr, "Erreur lors de l'écriture des dimensions de Q.\n");
        fclose(file);
        exit(EXIT_FAILURE);
    }

    // Ecriture de Q
    for (uint64_t i = 0; i < Q->m; i++) {
        if (fwrite(Q->values[i], sizeof(double), Q->n, file) != Q->n) {
            fprintf(stderr, "Erreur lors de l'écriture des valeurs de la matrice Q.\n");
            fclose(file);
            exit(EXIT_FAILURE);
        }
    }

    // Ecriture de R
    for (uint64_t i = 0; i < R->m; i++) {
        if (fwrite(R->values[i], sizeof(double), R->n, file) != R->n) {
            fprintf(stderr, "Erreur lors de l'écriture des valeurs de la matrice R.\n");
            fclose(file);
            exit(EXIT_FAILURE);
        }
    }
}