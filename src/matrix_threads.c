#include "../headers/matrix.h"
#include "../headers/vector.h"
#include "../headers/matrix_threads.h"
#include "../headers/error.h"
#include <math.h>
#include <inttypes.h>

#define SUCCESS 0
#define FAILURE 1

void* transp_thread(void* arg) {
    
    thread_data_transp* data = (thread_data_transp*)arg;
    uint64_t n = data->A->n;

    for (uint64_t i = data->start_row; i < data->end_row; i++) {
        for (uint64_t j = 0; j < n; j++) {
            data->T->values[j][i] = data->A->values[i][j];
        }
    }
    pthread_exit((void*) SUCCESS);
}

void* add_m_m_thread(void* arg){
    
    thread_data_m_m* data = (thread_data_m_m*)arg;
    uint64_t n = data->A->n;

    if(data->A->m != data->B->m ||data->A->m != data->C->m ||
        data->A->n != data->B->n || data->A->n != data->C->n){
            pthread_exit((void*) FAILURE);
        }

    for(uint64_t i = data->start_row; i < data->end_row; i++) {
        for(uint64_t j = 0; j < n; j++) {
            data->C->values[i][j] = data->A->values[i][j] + data->B->values[i][j];
        }
    }
    pthread_exit((void*) SUCCESS);

}

void* sub_m_m_thread(void* arg){
    thread_data_m_m* data = (thread_data_m_m*)arg;
    uint64_t n = data->A->n;

    if(data->A->m != data->B->m ||data->A->m != data->C->m ||
        data->A->n != data->B->n || data->A->n != data->C->n){
            free_matrix(data->A);
            free_matrix(data->B);
            free_matrix(data->C);
            free(data);
            handle_error(ERROR_SIZE_MISMATCH);
        }

    for(uint64_t i = data->start_row; i < data->end_row; i++) {
        for(uint64_t j = 0; j < n; j++) {
            data->C->values[i][j] = data->A->values[i][j] - data->B->values[i][j];
        }
    }
    pthread_exit((void*) SUCCESS);

}

void* mult_m_m_thread(void* arg){

    thread_data_m_m* data = (thread_data_m_m*) arg;
    uint64_t n = data->A->n;
    uint64_t o = data->B->n;

    if (data->A->n != data->B->m || 
        data->A->m != data->C->m || data->B->n != data->C->n) {
        handle_error(ERROR_SIZE_MISMATCH);
    }   

    // Taille des blocs (permet d'optimiser le cache)
    uint64_t block_size = 64;

    // Division de la matrice par blocs
    for (uint64_t i = data->start_row; i < data->end_row; i += block_size) {
        for (uint64_t j = 0; j < o; j += block_size) {
            // Calcul des blocs A et B
            for (uint64_t bi = i; bi < i + block_size && bi < data->A->m; bi++) {
                for (uint64_t bj = j; bj < j + block_size && bj < o; bj++) {
                    for (uint64_t k = 0; k < n; k++) {
                        data->C->values[bi][bj] += data->A->values[bi][k] * data->B->values[k][bj];
                    }
                }
            }
        }
    }
    pthread_exit((void*) SUCCESS);

}

void* mult_m_v_thread(void* arg){
    
    thread_data_m_v* data = (thread_data_m_v*) arg;
    uint64_t n = data->A->n;
    if(data->A->n != data->B->m || data->A->m != data->C->m){
        free_matrix(data->A);
        free_vector(data->B);
        free_vector(data->C);
        free(data);
        handle_error(ERROR_SIZE_MISMATCH);
    }

    for (uint64_t i = data->start_row; i < data->end_row; i++) {
        double res = 0;
        for (uint64_t j = 0; j < n; j++) {
            res += data->A->values[i][j] * data->B->values[j];
        }
        data->C->values[i] = res;
    }
    pthread_exit((void*) SUCCESS);
    
}

