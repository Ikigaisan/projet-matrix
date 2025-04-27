#include "../headers/vector.h"
#include "../headers/vector_threads.h"
#include <math.h>
#include <inttypes.h>
#include "../headers/error.h"

#define SUCCESS 0


void* add_v_v_thread(void *arg) {
    thread_data_v_v *data = (thread_data_v_v *)arg;  // On "déballe" le arg

    if (data == NULL) {
        handle_error(ERROR_ALLOC_STRUCT);
    }

    if (data->x == NULL || data->y == NULL || data->z == NULL){
        handle_error(ERROR_ALLOC_VALUES);
    }

    if (data->start_idx >= data->x->m || data->end_idx > data->x->m || data->start_idx >= data->end_idx) {
        handle_error(ERROR_INDEX_OUT_OF_BOUNDS); //Permet d'éviter d'ecrire en dehors du tableau
    }

    if(data->x->m != data->y->m || data->x->m != data->z->m){
        free_vector(data->x);
        free_vector(data->y);
        free_vector(data->z);
        free(data);
        handle_error(ERROR_SIZE_MISMATCH);
    }

    for (uint64_t i = data->start_idx; i < data->end_idx; i++) {
        data->z->values[i] = data->x->values[i] + data->y->values[i];
    }

    pthread_exit((void*) SUCCESS);
}

void* sub_v_v_thread (void *arg) {
    thread_data_v_v *data = (thread_data_v_v *)arg;

    if (data == NULL) {
        handle_error(ERROR_NULL_POINTER);
    }

    if (data->x == NULL || data->y == NULL || data->z == NULL){
        handle_error(ERROR_ALLOC_VALUES);
    }

    if (data->start_idx >= data->x->m || data->end_idx > data->x->m || data->start_idx >= data->end_idx) {
        handle_error(ERROR_INDEX_OUT_OF_BOUNDS); //Permet d'éviter d'ecrire en dehors du tableau
    }

    if(data->x->m != data->y->m || data->x->m != data->z->m){
        free_vector(data->x);
        free_vector(data->y);
        free_vector(data->z);
        free(data);
        handle_error(ERROR_SIZE_MISMATCH);
    }

    for(uint64_t i = data->start_idx; i < data->end_idx; i++) {
        data->z->values[i] = data->x->values[i] - data->y->values[i];
    }

    pthread_exit((void*) SUCCESS); 
}

