#include "../headers/vector.h"
#include "../headers/vector_threads.h"
#include <math.h>
#include <inttypes.h>


void* add_v_v_thread(void *arg) {
    thread_data_v_v *data = (thread_data_v_v *)arg;  // On récupère les données du thread

    // Vérifie que les vecteurs ont la même taille
    if(data->x->m != data->y->m || data->x->m != data->z->m){
        fprintf(stderr, "Tailles des vecteurs invalides : x(%" PRIu64 ") y(%" PRIu64 ") z(%" PRIu64 ")",
                data->x->m, data->y->m, data->z->m);
        free_vector(data->x);
        free_vector(data->y);
        free_vector(data->z);
        free(data);
        exit(EXIT_FAILURE);
    }

    // Calcul de l'addition sur la portion assignée
    for (uint64_t i = data->start_idx; i < data->end_idx; i++) {
        data->z->values[i] = data->x->values[i] + data->y->values[i];
    }

    return NULL;
}


void* sub_v_v_thread (void *arg) {
    thread_data_v_v *data = (thread_data_v_v *)arg;

    // Vérifie que les vecteurs ont la même taille
    if(data->x->m != data->y->m || data->x->m != data->z->m){
        fprintf(stderr, "Tailles des vecteurs invalides : x(%" PRIu64 ") y(%" PRIu64 ") z(%" PRIu64 ")",
                data->x->m, data->y->m, data->z->m);
        free_vector(data->x);
        free_vector(data->y);
        free_vector(data->z);
        free(data);
        exit(EXIT_FAILURE);
    }

    // Calcul de la soustraction sur la portion assignée
    for(uint64_t i = data->start_idx; i < data->end_idx; i++) {
        data->z->values[i] = data->x->values[i] - data->y->values[i];
    }

    return NULL;
}
