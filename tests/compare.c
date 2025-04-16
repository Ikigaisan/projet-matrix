#include <time.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <inttypes.h>

#include "../headers/matrix.h"
#include "../headers/vector.h"
#include "../headers/file.h"
#include "../headers/vector_threads.h"
#include "../headers/matrix_threads.h"


void compare_add_v_v(uint64_t size, int num_threads){
    struct timespec start, end;
    double elapsed_mono;
    double elapsed_multi;

    vector *x = init_vector(size);
    vector *y = init_vector(size);
    vector *z_mono = init_vector(size);
    vector *z_multi = init_vector(size);

    for(uint64_t i = 0; i<size; i++){
        x->values[i] = rand()/2;
        x->values[i] = rand()/2;
    }
    // Mono 
    clock_gettime(CLOCK_MONOTONIC, &start);
    add_v_v(x,y,z_mono);
    clock_gettime(CLOCK_MONOTONIC, &end);
    elapsed_mono = (end.tv_sec - start.tv_sec) + (end.tv_nsec - start.tv_nsec) / 1e9;


    // Multi

    pthread_t threads[num_threads];
    thread_data_v_v data[num_threads];
    uint64_t chunk_size = size/num_threads;

    clock_gettime(CLOCK_MONOTONIC, &start);
    for(u_int64_t i = 0; i< num_threads; i++){
        data[i].x = x;
        data[i].y = y;
        data[i].z = z_multi;
        data[i].start_idx = i*chunk_size;
        data[i].end_idx = (i==num_threads - 1) ? size:(i+1)*chunk_size;
        pthread_create(&threads[i], NULL, add_v_v_thread, &data[i]);
    }
    clock_gettime(CLOCK_MONOTONIC, &end);
    elapsed_multi = (end.tv_sec - start.tv_sec) + (end.tv_nsec - start.tv_nsec) / 1e9;

    // COmparaison
    printf("Pour la fonction [add] :\n");
    printf(" - Le monothread a mis %f s \n",elapsed_mono);
    printf("- Le multithread a mis %f s\n", elapsed_multi);
    
    if(elapsed_multi>elapsed_mono){
        double diff = elapsed_multi - elapsed_mono;
        double pourcent = (diff/elapsed_mono) * 100.0;
        printf("Le monothread est plus rapide de %.2f%%\n", pourcent);
    }else{
        double diff = elapsed_mono - elapsed_multi;
        double pourcent = (diff/elapsed_multi) * 100.0;
        printf("Le multithread est plus rapide de %.2f%%\n", pourcent);
    }

    free_vector(x);
    free_vector(y);
    free_vector(z_mono);
    free_vector(z_multi);

}

int main(int argc, char *argv[]){
    uint64_t size = 10000;
    if (argc < 2) {
        fprintf(stderr, "Usage: %s <nombre_de_threads>\n", argv[0]);
        return EXIT_FAILURE;
    }

    int num_threads = atoi(argv[1]);  // Convertit le string en int

    if (num_threads <= 0) {
        fprintf(stderr, "Nombre de threads invalide : %s\n", argv[1]);
        return EXIT_FAILURE;
    }

    printf("Lancement avec %d threads\n", num_threads);

    compare_add_v_v(10000, num_threads);

    return 0;
}
