#include <errno.h>
#include <getopt.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <inttypes.h>
#include <math.h>
#include "../headers/file.h"
#include "../headers/matrix.h"
#include "../headers/vector.h"
#include "../headers/vector_threads.h"
#include "../headers/matrix_threads.h"
#include <pthread.h>

typedef struct {
    bool verbose;
    uint8_t nb_threads;
    FILE *output_stream;
    uint8_t deg;
    char *op;
    FILE *input_file_A;
    FILE *input_file_B;
} args_t;

void usage(char *prog_name) {
    fprintf(stderr, "UTILISATION :\n");
    fprintf(stderr, "   %s [OPTIONS] name_op input_file_A [input_file_B]\n",
            prog_name);
    fprintf(stderr, "   name_op : nom de l'opération à effectuer.\n");
    fprintf(stderr,
            "   input_file_A : chemin relatif ou absolu vers le fichier "
            "contenant le "
            "premier vecteur/matrice de l'opération, au format binaire.\n");
    fprintf(stderr,
            "   input_file_B : optionnel ; chemin relatif ou absolu vers "
            "le fichier contenant le second vecteur/matrice de "
            "l'opération s'il y en a deux, au format binaire.\n");
    fprintf(stderr, "   -v : autorise les messages de debug sur la sortie "
                    "d'erreur standard. Défaut : false.\n");
    fprintf(stderr,
            "   -n n_threads : spécifie le nombre de threads qui peuvent "
            "être utilisés. Défaut : 1.\n");
    fprintf(stderr,
            "   -f output_stream : chemin relatif ou absolu vers le "
            "fichier qui contiendra le vecteur ou la matrice résultante "
            "au format binaire. Défaut : stdout.\n");
    fprintf(stderr, "   -d degree (juste pour lstsq) : degré du polynôme "
                    "d'interpolation (uint8_t). Défaut : 1.\n");
}

int parse_args(args_t *args, int argc, char **argv) {
    // Valeurs par défaut
    args->verbose = false; 
    args->nb_threads = 1;
    args->output_stream = stdout;
    args->deg = 1; 
    args->op = NULL; 
    args->input_file_A = NULL;
    args->input_file_B = NULL;

    int opt;
    while ((opt = getopt(argc, argv, "hn:vf:d:")) != -1) {
        switch (opt) {
        case 'n':
            args->nb_threads = atoi(optarg);
            if (args->nb_threads == 0) {
                fprintf(stderr,
                        "Le nombre de threads doit être supérieur à 0 !\n");
                exit(EXIT_FAILURE);
            }
            break;
        case 'v':
            args->verbose = true;
            break;
        case 'f':
            if (NULL == (args->output_stream = fopen(optarg, "w+"))) {
                fprintf(stderr,
                        "Impossible d'ouvrir le fichier de sortie %s : %s\n",
                        optarg, strerror(errno));
                exit(EXIT_FAILURE);
            }
            break;
        case 'd':
            args->deg = atoi(optarg);
            if (args->deg < 0) {
                fprintf(stderr,
                        "Le degré de l'approximation doit être positif !\n");
                exit(EXIT_FAILURE);
            }
            break;
        case 'h':
            usage(argv[0]);
            exit(EXIT_SUCCESS);
        default:
            usage(argv[0]);
            exit(EXIT_FAILURE);
        }
    }

    if (argc == optind || argc == optind + 1) {
        fprintf(stderr, "Vous devez fournir une opération et les fichiers "
                        "d'instances correspondant.\n");
        exit(EXIT_FAILURE);
    }

    args->op = argv[optind++];
    if (NULL == (args->input_file_A = fopen(argv[optind++], "r"))) {
        fprintf(stderr,
                "Impossible d'ouvrir le premier fichier d'entrée %s : %s\n",
                argv[optind - 1], strerror(errno));
        exit(EXIT_FAILURE);
    }

    if (strcmp(args->op, "add_v_v") == 0 || strcmp(args->op, "sub_v_v") == 0 ||
        strcmp(args->op, "dot_prod") == 0 || strcmp(args->op, "add_m_m") == 0 ||
        strcmp(args->op, "sub_m_m") == 0 || strcmp(args->op, "mult_m_v") == 0 ||
        strcmp(args->op, "mult_m_m") == 0 || strcmp(args->op, "norm") == 0 ||
        strcmp(args->op, "back_sub") == 0 || strcmp(args->op, "lstsq") == 0 ||
        strcmp(args->op, "QR") == 0 || strcmp(args->op, "transp")==0) {
        if (optind == argc) {
            fprintf(stderr,
                    "Vous devez fournir un second fichier d'instance pour "
                    "cette opération.\n");
            exit(EXIT_FAILURE);
        }
        if (NULL == (args->input_file_B = fopen(argv[optind], "r"))) {
            fprintf(stderr,
                    "Impossible d'ouvrir le second fichier d'entrée %s : %s\n",
                    argv[optind], strerror(errno));
            exit(EXIT_FAILURE);
        }
    }

    return 0;
}

int main(int argc, char **argv) {
    args_t *args = (args_t *)malloc(sizeof(args_t));
    if (args == NULL) {
        exit(EXIT_FAILURE);
    }
    parse_args(args, argc, argv);

    if (strcmp(args->op, "add_v_v") == 0) {
        vector *x = read_vector(args->input_file_A);
        if (args->verbose) {
            printf("vector x : \n");
            print_vector(x);
        }
        vector *y = read_vector(args->input_file_B);
        if (args->verbose) {
            printf("vector y : \n");
            print_vector(y);
        }
        vector *z = init_vector(x->m); // Vecteur qui contiendra x + y

        if(x->m != y->m){
            fprintf(stderr, "Erreur : les vecteurs x et y doivent avoir la même taille.\n");
            exit(EXIT_FAILURE);
        }else{
            pthread_t threads[args->nb_threads];
        thread_data_v_v thread_data[args->nb_threads];
        size_t chunk_size = x->m / args->nb_threads;

        // Création des threads qui utilisent la fonction add_v_v_thread
        for(uint64_t i = 0; i< args->nb_threads; i++){
            thread_data[i].x = x;
            thread_data[i].y = y;
            thread_data[i].z = z;
            thread_data[i].start_idx = i * chunk_size;
            thread_data[i].end_idx = (i == args->nb_threads -1)? x->m : (i+1)*chunk_size;

            pthread_create(&threads[i],NULL,add_v_v_thread, &thread_data[i]);
        }

        for(uint64_t i = 0; i < args->nb_threads; i++){
            pthread_join(threads[i], NULL);
        }

        }

        
        if (args->output_stream == stdout) {
            printf("Résultat de l'addition entre les deux vecteurs' : \n");
            print_vector(z);
        } else {
            write_vector(z, args->output_stream);
            if(args->verbose){
                printf("Résultat de l'addition entre les deux vecteurs' : \n");
                print_vector(z);
            }
        }
        free_vector(x);
        free_vector(y);
        free_vector(z);
    }else if (strcmp(args->op, "sub_v_v") == 0) {
        vector *x = read_vector(args->input_file_A);
        if (args->verbose) {
            printf("vector x : \n");
            print_vector(x);
        }
        vector *y = read_vector(args->input_file_B);
        if (args->verbose) {
            printf("vector y : \n");
            print_vector(y);
        }
        vector *z = init_vector(x->m);

        if(x->m != y->m){
            fprintf(stderr, "Erreur : les vecteurs x et y doivent avoir la même taille.\n");
            exit(EXIT_FAILURE);
        }else{
            pthread_t threads[args->nb_threads]; // tableau de threads en mode threadpool mais de nb_threads --> création de nb_threads
            thread_data_v_v thread_data[args->nb_threads]; // permet de données aux threads les données qu'ils vont traîter
            size_t chunk_size = x->m / args->nb_threads; // défini la répartition dans les différents threads
            
            // sub_v_v(x, y, z); on le supprime parce que mtn on passe en multithreads et plus en monothread ? 
            // Création des threads qui utilisent la fonction sub_v_v_thread
    
            for(uint64_t i = 0; i < args->nb_threads; i++) {
                thread_data[i].x = x;
                thread_data[i].y = y;
                thread_data[i].z = z;
                thread_data[i].start_idx = i*chunk_size;
                thread_data[i].end_idx = (i == args->nb_threads-1)? x->m : (i+1)*chunk_size; // x->m veut dire qu'on prend tous les éléments restant
    
                pthread_create(&threads[i], NULL, sub_v_v_thread, &thread_data[i]);
    
            }
    
            for(uint64_t i = 0; i < args->nb_threads; i++){
                pthread_join(threads[i], NULL);
            }
        }

    
        if (args->output_stream == stdout) {
            printf("Résultat de la soustraction entre les deux vecteurs : \n");
            print_vector(z);
        } else {
            write_vector(z, args->output_stream);
            if(args->verbose){
                printf("Résultat de la soustraction entre les deux vecteurs : \n");
                print_vector(z);
            }
        }
        free_vector(x);
        free_vector(y);
        free_vector(z);
    } else if (strcmp(args->op, "add_m_m") == 0) {
        matrix *A = read_matrix(args->input_file_A);
        if (args->verbose) {
            printf("matrix A : \n");
            print_matrix(A);
        }
        matrix *B = read_matrix(args->input_file_B);
        if (args->verbose) {
            printf("matrix B : \n");
            print_matrix(B);
        }
        matrix *C = init_matrix(A->m, A->n);
        // add_m_m(A, B, C); idem 

        if (A->m != B->m || A->n != B->n) {
            fprintf(stderr, "Erreur : les matrices A et B doivent avoir la même taille.\n");
            exit(EXIT_FAILURE);
        }else{
            pthread_t threads[args->nb_threads];
            thread_data_m_m thread_data[args->nb_threads]; 
            size_t chunk_size = A->m / args->nb_threads;

            for(uint64_t i = 0; i < args->nb_threads; i++){
                thread_data[i].A = A;
                thread_data[i].B = B;
                thread_data[i].C = C;
                thread_data[i].start_row = i * chunk_size;
                thread_data[i].end_row = (i == args->nb_threads-1)? A->m : (i+1)*chunk_size;

                pthread_create(&threads[i], NULL, add_m_m_thread, &thread_data[i]);

            }

            for(uint64_t i = 0; i < args->nb_threads; i++){
                pthread_join(threads[i], NULL);
            }
        } 
    
    
        if (args->output_stream == stdout) {
            printf("Résultat de l'addition entre les deux matrices : \n");
            print_matrix(C);
        } else {
            write_matrix(C, args->output_stream);
            if(args->verbose){
                printf("Résultat de l'addition entre les deux matrices': \n");
                print_matrix(C);
            }
        }
        free_matrix(A);
        free_matrix(B);
        free_matrix(C);

    }else if (strcmp(args->op, "sub_m_m") == 0) {
        matrix *A = read_matrix(args->input_file_A);
        if (args->verbose) {
            printf("matrix A : \n");
            print_matrix(A);
        }
        matrix *B = read_matrix(args->input_file_B);
        if (args->verbose) {
            printf("matrix B : \n");
            print_matrix(B);
        }
        matrix *C = init_matrix(A->m, A->n);
        //sub_m_m(A, B, C);

        if (A->m != B->m || A->n != B->n) {
            fprintf(stderr, "Erreur : les matrices A et B doivent avoir la même taille.\n");
            exit(EXIT_FAILURE);
        }else{
            pthread_t threads[args->nb_threads];
            thread_data_m_m thread_data[args->nb_threads]; 
            size_t chunk_size = A->m / args->nb_threads;

            for(uint64_t i = 0; i < args->nb_threads; i++){
                thread_data[i].A = A;
                thread_data[i].B = B;
                thread_data[i].C = C;
                thread_data[i].start_row = i * chunk_size;
                thread_data[i].end_row = (i == args->nb_threads-1)? A->m : (i+1)*chunk_size;

                pthread_create(&threads[i], NULL, sub_m_m_thread, &thread_data[i]);

            }

            for(uint64_t i = 0; i < args->nb_threads; i++){
                pthread_join(threads[i], NULL);
            }
        } 
    
        if (args->output_stream == stdout) {
            printf("Résultat de la soustraction entre les deux matrices : \n");
            print_matrix(C);
        } else {
            write_matrix(C, args->output_stream);
            if(args->verbose){
                printf("Résultat de la soustraction entre les deux matrices : \n");
                print_matrix(C);
            }
        }
        free_matrix(A);
        free_matrix(B);
        free_matrix(C);

    }else if (strcmp(args->op, "dot_prod") == 0) {
        vector *x = read_vector(args->input_file_A);
        if (args->verbose) {
            printf("vector x : \n");
            print_vector(x);
        }
        vector *y = read_vector(args->input_file_B);
        if (args->verbose) {
            printf("vector y : \n");
            print_vector(y);
        }

        double result;
        dot_prod(x, y, &result);

        if(args->output_stream == stdout) {   
            printf("Produit scalaire : %f\n", result);
        } else {
            write_double(result, args->output_stream);
            if(args->verbose){
                printf("Résultat du produit scalaire : %f\n", result);
            }
        }        

        free_vector(x);
        free_vector(y);
    } else if(strcmp(args->op, "norm") == 0) {
        vector *a = read_vector(args->input_file_A);
        if(args->verbose) {
            printf("Vector a :\n");
            print_vector(a);
        }

        double result;
        norm(a, &result);
        if(args->output_stream == stdout) {
            printf("Norme : %f\n", result);
        }else {
            write_double(result, args-> output_stream);
            if(args->verbose){
                printf("Résultat de la norme : %f\n", result);
            }
        }
    }else if(strcmp(args->op,"mult_m_m") == 0) {
        matrix *A = read_matrix(args->input_file_A);
        if (args->verbose) {
            printf("Matrix A : \n");
            print_matrix(A);
        }

        matrix *B = read_matrix(args->input_file_B);
        if (args->verbose) {
            printf("Matrix B : \n");
            print_matrix(B);
        }
        matrix *C = init_matrix(A->m, B->n);
        // mult_m_m(A, B, Result);

        if (A->n != B->m) {
            fprintf(stderr, "Erreur : dimensions incompatibles.\n");
            exit(EXIT_FAILURE);
        }else{
            pthread_t threads[args->nb_threads];
            thread_data_m_m thread_data[args->nb_threads]; 
            size_t chunk_size = A->m / args->nb_threads;

            for(uint64_t i = 0; i < args->nb_threads; i++){
                thread_data[i].A = A;
                thread_data[i].B = B;
                thread_data[i].C = C;
                thread_data[i].start_row = i * chunk_size;
                thread_data[i].end_row = (i == args->nb_threads-1)? A->m : (i+1)*chunk_size;

                pthread_create(&threads[i], NULL, mult_m_m_thread, &thread_data[i]);

            }

            for(uint64_t i = 0; i < args->nb_threads; i++){
                pthread_join(threads[i], NULL);
            }
        }
        
        if(args->output_stream == stdout){
            printf("Resultat de la multiplication :\n");
            print_matrix(C);
        }else{
            write_matrix(C, args->output_stream);
            if(args->verbose){
                printf("Résultat de la multiplication: \n");
                print_matrix(C);
            }
        }
        free_matrix(A);
        free_matrix(B);
        free_matrix(C);

    }else if (strcmp(args->op, "mult_m_v")== 0)
    {
        matrix *A = read_matrix(args->input_file_A);
        if(args->verbose){
            printf("Matrix A :\n");
            print_matrix(A);
        }

        vector *B = read_vector(args->input_file_B);
        if(args->verbose){
            printf("Vector b:\n");
            print_vector(B);
        }

        vector *C = init_vector(A->m);
        // mult_m_v(A, B, C);

        if (A->n != B->m) {
            fprintf(stderr, "Erreur : dimensions incompatibles.\n");
            exit(EXIT_FAILURE);
        }else{
            pthread_t threads[args->nb_threads];
            thread_data_m_v thread_data[args->nb_threads]; 
            size_t chunk_size = A->m / args->nb_threads;

            for(uint64_t i = 0; i < args->nb_threads; i++){
                thread_data[i].A = A;
                thread_data[i].B = B;
                thread_data[i].C = C;
                thread_data[i].start_row = i * chunk_size;
                thread_data[i].end_row = (i == args->nb_threads-1)? A->m : (i+1)*chunk_size;

                pthread_create(&threads[i], NULL, mult_m_v_thread, &thread_data[i]);

            }

            for(uint64_t i = 0; i < args->nb_threads; i++){
                pthread_join(threads[i], NULL);
            }
        }


        if(args->output_stream == stdout){
            printf("Résultat de la multiplication entre la matrice et le vecteur : \n");
            print_vector(C);
        }else{
            write_vector(C, args->output_stream);
            if(args->verbose){
                printf("Résultat de la multiplication entre la matrice et le vecteur : \n");
                print_vector(C);
            }
        }
        free_matrix(A);
        free_vector(B);
        free_vector(C);

    }else if(strcmp(args->op, "transp")== 0){
        matrix *A = read_matrix(args->input_file_A);
        if(args->verbose){
            printf("Matrix A :\n");
            print_matrix(A);
        }
        transp(A);
        if(args->output_stream == stdout){
            printf("Résultat de la transposée :\n");
            print_matrix(A);
        }else{
            write_matrix(A, args->output_stream);
            if(args->verbose){
                printf("Résultat de la transposée : \n");
                print_matrix(A);
            }
        }
        free_matrix(A);
    }else {
        fprintf(stderr, "Cette opération n'est pas implémentée...\n");
        exit(EXIT_FAILURE);
    }

    fclose(args->input_file_A);
    if (strcmp(args->op, "add_v_v") == 0 || strcmp(args->op, "sub_v_v") == 0 ||
        strcmp(args->op, "dot_prod") == 0 || strcmp(args->op, "add_m_m") == 0 ||
        strcmp(args->op, "sub_m_m") == 0 || strcmp(args->op, "mult_m_v") == 0 ||
        strcmp(args->op, "mult_m_m") == 0 ||
        strcmp(args->op, "back_sub") == 0 || strcmp(args->op, "lstsq") == 0) {
        fclose(args->input_file_B);
    }
    if (args->output_stream != stdout) {
        fclose(args->output_stream);
    }
    exit(EXIT_SUCCESS);
}