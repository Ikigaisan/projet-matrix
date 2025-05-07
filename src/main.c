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

// Structure pour stocker les arguments de la ligne de commande
typedef struct {
    bool verbose;             // Mode verbeux : affiche les messages de debug sur stderr
    uint8_t nb_threads;       // Nombre de threads à utiliser
    FILE *output_stream;      // Fichier de sortie (stdout par défaut)
    uint8_t deg;              // Degré du polynôme pour lstsq (interpolation)
    char *op;                 // Nom de l'opération à effectuer
    FILE *input_file_A;       // Premier fichier d'entrée (vecteur/matrice)
    FILE *input_file_B;       // Deuxième fichier d'entrée, optionnel (si l'opération en nécessite deux)
} args_t;

// Affiche l'aide et la syntaxe d'utilisation
void usage(char *prog_name) {
    fprintf(stderr, "UTILISATION :\n");
    fprintf(stderr, "   %s [OPTIONS] name_op input_file_A [input_file_B]\n", prog_name);
    fprintf(stderr, "   name_op : nom de l'opération à effectuer.\n");
    fprintf(stderr,
            "   input_file_A : chemin relatif ou absolu vers le fichier contenant le "
            "premier vecteur/matrice de l'opération, au format binaire.\n");
    fprintf(stderr,
            "   input_file_B : optionnel ; chemin relatif ou absolu vers le fichier contenant le second vecteur/matrice de "
            "l'opération s'il y en a deux, au format binaire.\n");
    fprintf(stderr, "   -v : autorise les messages de debug sur la sortie d'erreur standard. Défaut : false.\n");
    fprintf(stderr,
            "   -n n_threads : spécifie le nombre de threads qui peuvent être utilisés. Défaut : 1.\n");
    fprintf(stderr,
            "   -f output_stream : chemin relatif ou absolu vers le fichier qui contiendra le vecteur ou la matrice résultante au format binaire. Défaut : stdout.\n");
    fprintf(stderr, "   -d degree (juste pour lstsq) : degré du polynôme d'interpolation (uint8_t). Défaut : 1.\n");
}

// Fonction de parsing des arguments de la ligne de commande
int parse_args(args_t *args, int argc, char **argv) {
    // Initialisation des valeurs par défaut
    args->verbose = false;
    args->nb_threads = 1;
    args->output_stream = stdout;
    args->deg = 1; 
    args->op = NULL; 
    args->input_file_A = NULL;
    args->input_file_B = NULL;

    int opt;

    // Analyse des options passées en ligne de commande grâce à getopt
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

    // Vérification qu'il y a suffisamment d'arguments restants
    if (argc == optind || argc == optind + 1) {
        fprintf(stderr, "Vous devez fournir une opération et les fichiers d'instances correspondant.\n");
        exit(EXIT_FAILURE);
    }

    // Récupération du nom de l'opération et ouverture du premier fichier
    args->op = argv[optind++];
    if (NULL == (args->input_file_A = fopen(argv[optind++], "r"))) {
        fprintf(stderr,
                "Impossible d'ouvrir le premier fichier d'entrée %s : %s\n",
                argv[optind - 1], strerror(errno));
        exit(EXIT_FAILURE);
    }

    // Certaines opérations nécessitent un deuxième fichier d'entrée
    if (strcmp(args->op, "add_v_v") == 0 || strcmp(args->op, "sub_v_v") == 0 ||
        strcmp(args->op, "dot_prod") == 0 || strcmp(args->op, "add_m_m") == 0 ||
        strcmp(args->op, "sub_m_m") == 0 || strcmp(args->op, "mult_m_v") == 0 ||
        strcmp(args->op, "mult_m_m") == 0 || strcmp(args->op, "qr") == 0 ||
        strcmp(args->op, "back_sub") == 0 || strcmp(args->op, "lstsq") == 0) {
        if (optind == argc) {
            fprintf(stderr,
                    "Vous devez fournir un second fichier d'instance pour cette opération.\n");
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

// Fonction main //

int main(int argc, char **argv) {
    // Allocation dynamique de la structure args
    args_t *args = (args_t *)malloc(sizeof(args_t));
    if (args == NULL) {
        exit(EXIT_FAILURE);
    }
    // Analyse et stockage des arguments de la ligne de commande
    parse_args(args, argc, argv);

    // VECTEURS //

    // Opération : Addition de deux vecteurs
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
        // Initialisation du vecteur résultat qui contiendra x + y
        vector *z = init_vector(x->m);

        add_v_v(x,y,z);

        // Affichage et/ou écriture du résultat
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
    
    }

    // Opération : Soustraction de deux vecteurs
    else if (strcmp(args->op, "sub_v_v") == 0) {
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
            sub_v_v(x,y,z);
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
    }

    // Opération : Addition de deux matrices
    else if (strcmp(args->op, "add_m_m") == 0) {
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
    }

    // Opération : Soustraction de deux matrices
    else if (strcmp(args->op, "sub_m_m") == 0) {
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
    }

    // Opération : Produit scalaire de deux vecteurs
    else if (strcmp(args->op, "dot_prod") == 0) {
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
        if (args->output_stream == stdout) {   
            printf("Produit scalaire : %f\n", result);
        } else {
            write_double(result, args->output_stream);
            if(args->verbose){
                printf("Résultat du produit scalaire : %f\n", result);
            }
        }
        free_vector(x);
        free_vector(y);
    }

    // Opération : Calcul de la norme 2 d'un vecteur
    else if (strcmp(args->op, "norm") == 0) {
        vector *a = read_vector(args->input_file_A);
        if (args->verbose) {
            printf("Vector a :\n");
            print_vector(a);
        }
        double result;
        norm(a, &result);
        if (args->output_stream == stdout) {
            printf("Norme : %f\n", result);
        } else {
            write_double(result, args->output_stream);
            if(args->verbose){
                printf("Résultat de la norme : %f\n", result);
            }
        }
        free_vector(a);
    }

    // Opération : Multiplication de deux matrices
    else if (strcmp(args->op, "mult_m_m") == 0) {
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

    // Opération : Multiplication d’une matrice par un vecteur
    }else if (strcmp(args->op, "mult_m_v") == 0) {
        matrix *A = read_matrix(args->input_file_A);
        if (args->verbose) {
            printf("Matrix A :\n");
            print_matrix(A);
        }
        vector *b = read_vector(args->input_file_B);
        if (args->verbose) {
            printf("Vector b:\n");
            print_vector(b);
        }

        vector *c = init_vector(A->m);
        // mult_m_v(A, B, C);

        if (A->n != b->m) {
            fprintf(stderr, "Erreur : dimensions incompatibles.\n");
            exit(EXIT_FAILURE);
        }else{
            pthread_t threads[args->nb_threads];
            thread_data_m_v thread_data[args->nb_threads]; 
            size_t chunk_size = A->m / args->nb_threads;

            for(uint64_t i = 0; i < args->nb_threads; i++){
                thread_data[i].A = A;
                thread_data[i].B = b;
                thread_data[i].C = c;
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
            print_vector(c);
        }else{
            write_vector(c, args->output_stream);
            if(args->verbose){
                printf("Résultat de la multiplication entre la matrice et le vecteur : \n");
                print_vector(c);
            }
        }
        free_matrix(A);
        free_vector(b);
        free_vector(c);
    }

    // Opération : Transposée d’une matrice (la fonction transp effectue la transposition sur place)
    else if (strcmp(args->op, "transp") == 0) {
        matrix *A = read_matrix(args->input_file_A);
        if (args->verbose) {
            printf("Matrix A :\n");
            print_matrix(A);
        }
    
        matrix* T = init_matrix(A->n, A->m); // Matrice transposée 
    
        pthread_t threads[args->nb_threads];
        thread_data_transp thread_data[args->nb_threads];
        uint64_t chunk_size = A->m / args->nb_threads; // Division par ligne pour la transposition
    
        // Création des threads qui utilisent la fonction transp_thread
        for (uint64_t i = 0; i < args->nb_threads; i++) {
            thread_data[i].A = A;
            thread_data[i].T = T;
            thread_data[i].start_row = i * chunk_size;
            thread_data[i].end_row = (i == args->nb_threads - 1) ? A->m : (i + 1) * chunk_size;
    
            pthread_create(&threads[i], NULL, transp_thread, &thread_data[i]);
        }
    
        for (uint64_t i = 0; i < args->nb_threads; i++) {
            pthread_join(threads[i], NULL);
        }
    
        // Libérer A pour éviter les fuites de mémoire suite au changement de dimensions 

        for (uint64_t i = 0; i < A->m; i++) {
            free(A->values[i]);
        }
        free(A->values);

        // Mise à jour de la matrice A avec la transposée
        A->m = T->m;
        A->n = T->n;
        A->values = T->values;
    
        free(T);
    
        if (args->output_stream == stdout) {
            printf("Résultat de la transposée :\n");
            print_matrix(A);
        } else {
            write_matrix(A, args->output_stream);
            if (args->verbose) {
                printf("Résultat de la transposée : \n");
                print_matrix(A);
            }
        }
        free_matrix(A);
    }

    // Opération : Substitution arrière
    else if (strcmp(args->op, "back_sub") == 0) {
        // Lecture de la matrice A et du vecteur b
        matrix *A = read_matrix(args->input_file_A);
        vector *b = read_vector(args->input_file_B);
        // Allocation du vecteur solution
        vector *result = init_vector(b->m);
        // Appel de la fonction de substitution arrière
        back_sub(b, A, result);
        if (args->output_stream == stdout) {
            printf("Résultat de la substitution arrière :\n");
            print_vector(result);
        } else {
            write_vector(result, args->output_stream);
        }
        free_matrix(A);
        free_vector(b);
        free_vector(result);
    }

    // Opération : Décomposition QR
    else if (strcmp(args->op, "qr") == 0) {
        // Lecture de la matrice A
        matrix *A = read_matrix(args->input_file_A);
        // Allocation des matrices Q et R
        // On suppose ici que Q est carrée de dimensions A->m x A->m et R est de dimensions A->m x A->n
        matrix *Q = init_matrix(A->m, A->m);
        matrix *R = init_matrix(A->m, A->n);
        // Calcul de la décomposition QR
   
        if (args->output_stream == stdout) {
            printf("Matrice Q :\n");
            print_matrix(Q);
            printf("Matrice R :\n");
            print_matrix(R);
        } else {
            write_matrix(Q, args->output_stream);
            write_matrix(R, args->output_stream);
        }
        free_matrix(A);
        free_matrix(Q);
        free_matrix(R);
    }
    else if (strcmp(args->op, "lstsq") == 0) {
        // Lecture de la matrice A et du vecteur b
        matrix *A = read_matrix(args->input_file_A);
        vector *b = read_vector(args->input_file_B);
        // Allocation du vecteur solution
        vector *result = init_vector(b->m);
        // Appel de la fonction de substitution arrière
        result =lstsq(b, A);
        if (args->output_stream == stdout) {
            printf("Résultat de la substitution arrière :\n");
            print_vector(result);
        } else {
            write_vector(result, args->output_stream);
        }
        free_matrix(A);
        free_vector(b);
        free_vector(result);
    }
    
    // Si l'opération demandée n'est pas implémentée
    else {
        fprintf(stderr, "Cette opération n'est pas implémentée...\n");
        exit(EXIT_FAILURE);
    }

    // Fermeture des fichiers ouverts
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
