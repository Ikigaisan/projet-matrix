#include <time.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <inttypes.h>
#include <unistd.h>

#include "../headers/matrix.h"
#include "../headers/vector.h"
#include "../headers/file.h"
#include "../headers/vector_threads.h"
#include "../headers/matrix_threads.h"

// Affiche les temps d'exécution et la différence entre mono et multi
void print_comparison(double elapsed_mono, double elapsed_multi){
    printf("- Le monothread a mis %f s \n", elapsed_mono);
    printf("- Le multithread a mis %f s\n", elapsed_multi);
    
    if (elapsed_multi > elapsed_mono) {
        double diff = elapsed_multi - elapsed_mono;
        double pourcent = (diff / elapsed_multi) * 100.0;
        printf("Le monothread est plus rapide de %.2f%%\n", pourcent);
    } else {
        double diff = elapsed_mono - elapsed_multi;
        double pourcent = (diff / elapsed_mono) * 100.0;
        printf("Le multithread est plus rapide de %.2f%%\n\n", pourcent);
    }
}

// Insère les résultats dans un fichier CSV
void insert_csv(const char *func_name, double elapsed_mono, double elapsed_multi, uint64_t size, int num_threads){
    FILE *file = fopen("results_compare.csv", "a");
    if(file != NULL){
        fprintf(file, "%s,%" PRIu64 ",%f,%f,%d\n", func_name, size, elapsed_mono, elapsed_multi, num_threads);
        fclose(file);
    }
}

// Compare add_v_v (mono vs multi)
void compare_add_v_v(uint64_t size, int num_threads, int mode_graph){
    struct timespec start, end;
    double elapsed_mono;
    double elapsed_multi;

    vector *x = init_vector(size);
    vector *y = init_vector(size);
    vector *z = init_vector(size);

    for(uint64_t i = 0; i<size; i++){
        x->values[i] = rand()/2;
        y->values[i] = rand()/2;
    }

    // Mesure du temps monothread
    clock_gettime(CLOCK_MONOTONIC, &start);
    add_v_v(x, y, z);
    clock_gettime(CLOCK_MONOTONIC, &end);
    elapsed_mono = (end.tv_sec - start.tv_sec) + (end.tv_nsec - start.tv_nsec) / 1e9;

    // Configuration et mesure multithread
    pthread_t threads[num_threads];
    thread_data_v_v data[num_threads];
    uint64_t chunk_size = size / num_threads;

    clock_gettime(CLOCK_MONOTONIC, &start);
    for(uint64_t i = 0; i < num_threads; i++){
        data[i].x = x;
        data[i].y = y;
        data[i].z = z;
        data[i].start_idx = i * chunk_size;
        data[i].end_idx = (i == num_threads - 1) ? size : (i + 1) * chunk_size;
        pthread_create(&threads[i], NULL, add_v_v_thread, &data[i]);
    }
    for (uint64_t i = 0; i < num_threads; i++) {
        pthread_join(threads[i], NULL);
    }
    clock_gettime(CLOCK_MONOTONIC, &end);
    elapsed_multi = (end.tv_sec - start.tv_sec) + (end.tv_nsec - start.tv_nsec) / 1e9;

    // Affichage ou export CSV
    if(mode_graph){
        insert_csv("add_v_v", elapsed_mono, elapsed_multi, size, num_threads);
    }else{
        printf("\nPour la fonction [add_v_v] avec des vecteurs de taille %" PRIu64 ":\n", size);
        print_comparison(elapsed_mono, elapsed_multi);
    }

    free_vector(x);
    free_vector(y);
    free_vector(z);
}

// Compare add_m_m (mono vs multi)
void compare_add_m_m(uint64_t size, int num_threads, int mode_graph){
    struct timespec start, end;
    double elapsed_mono;
    double elapsed_multi;

    matrix *A = init_matrix(size, size);
    matrix *B = init_matrix(size, size);
    matrix *C = init_matrix(size, size);

    for(uint64_t i = 0; i < size; i++){
        for(uint64_t j = 0; j < size; j++){
            A->values[i][j] = rand()/2;
            B->values[i][j] = rand()/2;
        }
    }

    // Monothread
    clock_gettime(CLOCK_MONOTONIC, &start);
    add_m_m(A, B, C);
    clock_gettime(CLOCK_MONOTONIC, &end);
    elapsed_mono = (end.tv_sec - start.tv_sec) + (end.tv_nsec - start.tv_nsec) / 1e9;

    // Multithread
    pthread_t threads[num_threads];
    thread_data_m_m thread_data[num_threads]; 
    size_t chunk_size = size / num_threads;

    clock_gettime(CLOCK_MONOTONIC, &start);
    for(uint64_t i = 0; i < num_threads; i++){
        thread_data[i].A = A;
        thread_data[i].B = B;
        thread_data[i].C = C;
        thread_data[i].start_row = i * chunk_size;
        thread_data[i].end_row = (i == num_threads - 1) ? size : (i + 1) * chunk_size;
        pthread_create(&threads[i], NULL, add_m_m_thread, &thread_data[i]);
    }
    for(uint64_t i = 0; i < num_threads; i++){
        pthread_join(threads[i], NULL);
    }
    clock_gettime(CLOCK_MONOTONIC, &end);
    elapsed_multi = (end.tv_sec - start.tv_sec) + (end.tv_nsec - start.tv_nsec) / 1e9;

    if(mode_graph){
        insert_csv("add_m_m", elapsed_mono, elapsed_multi, size, num_threads);
    }else{
        printf("\nPour la fonction [add_m_m] avec une taille de %" PRIu64 ":\n", size);
        print_comparison(elapsed_mono, elapsed_multi);
    }

    free_matrix(A);
    free_matrix(B);
    free_matrix(C);
}

// Compare transp (mono vs multi)
void compare_transp(uint64_t size, int num_threads, int mode_graph){
    struct timespec start, end;
    double elapsed_mono;
    double elapsed_multi;

    matrix *A_mono = init_matrix(size, size);
    matrix *A_multi = init_matrix(size, size);
    matrix *T_multi = init_matrix(size, size);

    for(uint64_t i = 0; i < size; i++){
        for(uint64_t j = 0; j < size; j++){
            double val = rand()/2;
            A_mono->values[i][j] = val;
            A_multi->values[i][j] = val;
        }
    }

    clock_gettime(CLOCK_MONOTONIC, &start);
    transp(A_mono);
    clock_gettime(CLOCK_MONOTONIC, &end);
    elapsed_mono = (end.tv_sec - start.tv_sec) + (end.tv_nsec - start.tv_nsec) / 1e9;

    clock_gettime(CLOCK_MONOTONIC, &start);
    pthread_t threads[num_threads];
    thread_data_transp thread_data[num_threads];
    uint64_t chunk_size = size / num_threads;

    for (uint64_t i = 0; i < num_threads; i++) {
        thread_data[i].A = A_multi;
        thread_data[i].T = T_multi;
        thread_data[i].start_row = i * chunk_size;
        thread_data[i].end_row = (i == num_threads - 1) ? size : (i + 1) * chunk_size;
        pthread_create(&threads[i], NULL, transp_thread, &thread_data[i]);
    }

    for (uint64_t i = 0; i < num_threads; i++) {
        pthread_join(threads[i], NULL);
    }
    clock_gettime(CLOCK_MONOTONIC, &end);
    elapsed_multi = (end.tv_sec - start.tv_sec) + (end.tv_nsec - start.tv_nsec) / 1e9;

    if(mode_graph){
        insert_csv("transp", elapsed_mono, elapsed_multi, size, num_threads);
    }else{
        printf("\nPour la fonction [transp] avec une taille de %" PRIu64 ":\n", size);
        print_comparison(elapsed_mono, elapsed_multi);
    }
}

// Compare mult_m_v (mono vs multi)
void compare_mult_m_v(uint64_t size, int num_threads, int mode_graph){
    struct timespec start, end;
    double elapsed_mono;
    double elapsed_multi;

    matrix *A = init_matrix(size, size);
    vector *b = init_vector(size);
    vector *c = init_vector(size);

    for(uint64_t i = 0; i < size; i++){
        b->values[i] = rand()/2;
        for(uint64_t j = 0; j < size; j++){
            A->values[i][j] = rand()/2;
        }
    }

    clock_gettime(CLOCK_MONOTONIC, &start);
    mult_m_v(A, b, c);
    clock_gettime(CLOCK_MONOTONIC, &end);
    elapsed_mono = (end.tv_sec - start.tv_sec) + (end.tv_nsec - start.tv_nsec) / 1e9;

    clock_gettime(CLOCK_MONOTONIC, &start);
    pthread_t threads[num_threads];
    thread_data_m_v thread_data[num_threads];
    uint64_t chunk_size = size / num_threads;

    for(uint64_t i = 0; i < num_threads; i++){
        thread_data[i].A = A;
        thread_data[i].B = b;
        thread_data[i].C = c;
        thread_data[i].start_row = i * chunk_size;
        thread_data[i].end_row = (i == num_threads - 1) ? size : (i + 1) * chunk_size;
        pthread_create(&threads[i], NULL, mult_m_v_thread, &thread_data[i]);
    }

    for(uint64_t i = 0; i < num_threads; i++){
        pthread_join(threads[i], NULL);
    }
    clock_gettime(CLOCK_MONOTONIC, &end);
    elapsed_multi = (end.tv_sec - start.tv_sec) + (end.tv_nsec - start.tv_nsec) / 1e9;

    if(mode_graph){
        insert_csv("mult_m_v", elapsed_mono, elapsed_multi, size, num_threads);
    }else{
        printf("\nPour la fonction [mult_m_v] avec une taille de %" PRIu64 ":\n", size);
        print_comparison(elapsed_mono, elapsed_multi);
    }

    free_matrix(A);
    free_vector(b);
    free_vector(c);
}

// Compare mult_m_m (mono vs multi)
void compare_mult_m_m(uint64_t size, int num_threads, int mode_graph){
    struct timespec start, end;
    double elapsed_mono;
    double elapsed_multi;

    matrix *A = init_matrix(size, size);
    matrix *B = init_matrix(size, size);
    matrix *C = init_matrix(size, size);

    for(uint64_t i = 0; i < size; i++){
        for(uint64_t j = 0; j < size; j++){
            A->values[i][j] = rand()/2;
            B->values[i][j] = rand()/2;
        }
    }

    clock_gettime(CLOCK_MONOTONIC, &start);
    mult_m_m(A, B, C);
    clock_gettime(CLOCK_MONOTONIC, &end);
    elapsed_mono = (end.tv_sec - start.tv_sec) + (end.tv_nsec - start.tv_nsec) / 1e9;

    clock_gettime(CLOCK_MONOTONIC, &start);
    pthread_t threads[num_threads];
    thread_data_m_m thread_data[num_threads]; 
    size_t chunk_size = size / num_threads;

    for(uint64_t i = 0; i < num_threads; i++){
        thread_data[i].A = A;
        thread_data[i].B = B;
        thread_data[i].C = C;
        thread_data[i].start_row = i * chunk_size;
        thread_data[i].end_row = (i == num_threads - 1) ? size : (i + 1) * chunk_size;
        pthread_create(&threads[i], NULL, mult_m_m_thread, &thread_data[i]);
    }

    for(uint64_t i = 0; i < num_threads; i++){
        pthread_join(threads[i], NULL);
    }
    clock_gettime(CLOCK_MONOTONIC, &end);
    elapsed_multi = (end.tv_sec - start.tv_sec) + (end.tv_nsec - start.tv_nsec) / 1e9;

    if(mode_graph){
        insert_csv("mult_m_m", elapsed_mono, elapsed_multi, size, num_threads);
    }else{
        printf("\nPour la fonction [mult_m_m] avec une taille de %" PRIu64 ":\n", size);
        print_comparison(elapsed_mono, elapsed_multi);
    }

    free_matrix(A);
    free_matrix(B);
    free_matrix(C);
}

// Point d'entrée du programme
int main(int argc, char *argv[]){
    if (argc < 3) {
        fprintf(stderr, "Usage: %s <nombre_de_threads> <mode d'affichage : console|graphique> \n", argv[0]);
        return EXIT_FAILURE;
    }

    int num_threads = atoi(argv[1]);
    int mode_graph = 0;

    if (num_threads <= 0) {
        fprintf(stderr, "Nombre de threads invalide : %s\n", argv[1]);
        return EXIT_FAILURE;
    }

    if(strcmp(argv[2], "graphique") == 0){
        mode_graph = 1;
        unlink("results_compare.csv");  // Supprime l'ancien fichier CSV
    }else if(strcmp(argv[2], "console") != 0){
        fprintf(stderr, "Mode invalide : %s (utiliser 'console' ou 'graphique')\n", argv[2]);
        return EXIT_FAILURE;
    }

    printf("Lancement avec %d threads\n", num_threads);


    // Lancement des comparaisons pour chaque taille
    for(uint64_t i = 10; i <= 1000; i += 10){
        compare_add_v_v(i, num_threads, mode_graph);
        compare_add_m_m(i, num_threads, mode_graph);
        compare_transp(i, num_threads, mode_graph);
        compare_mult_m_v(i, num_threads, mode_graph);
        compare_mult_m_m(i, num_threads, mode_graph);
    }
    for(uint64_t i = 1200; i <= 5000; i += 400){
        compare_add_v_v(i, num_threads, mode_graph);
        compare_add_m_m(i, num_threads, mode_graph);
        compare_transp(i, num_threads, mode_graph);
        compare_mult_m_v(i, num_threads, mode_graph);
        compare_mult_m_m(i, num_threads, mode_graph);
    }
    for(uint64_t i = 6000; i <= 10000; i += 2000){
        compare_add_v_v(i, num_threads, mode_graph);
        compare_add_m_m(i, num_threads, mode_graph);
        compare_transp(i, num_threads, mode_graph);
        compare_mult_m_v(i, num_threads, mode_graph);
        compare_mult_m_m(i, num_threads, mode_graph);
    }

    return 0;
}
