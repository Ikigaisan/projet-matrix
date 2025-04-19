#include "../headers/matrix.h"
#include "../headers/vector.h"
#include "../headers/matrix_threads.h"

void* transp_thread(void* arg) {
    
    thread_data_transp* data = (thread_data_transp*)arg;
    uint64_t n = data->A->n;

    for (uint64_t i = data->start_row; i < data->end_row; i++) {
        for (uint64_t j = 0; j < n; j++) {
            data->T->values[j][i] = data->A->values[i][j];
        }
    }
    return NULL;
}

void* add_m_m_thread(void* arg){
    
    thread_data_m_m* data = (thread_data_m_m*)arg;
    uint64_t n = data->A->n;

    if(data->A->m != data->B->m ||data->A->m != data->C->m ||
        data->A->n != data->B->n || data->A->n != data->C->n){
            fprintf(stderr, "Erreur : les matrices doivent être de la même taille.\n");
            free_matrix(data->A);
            free_matrix(data->B);
            free_matrix(data->C);
            free(data);
            exit(EXIT_FAILURE);
        }

    for(uint64_t i = data->start_row; i < data->end_row; i++) {
        for(uint64_t j = 0; j < n; j++) {
            data->C->values[i][j] = data->A->values[i][j] + data->B->values[i][j];
        }
    }
    return NULL;

}

void* sub_m_m_thread(void* arg){
    thread_data_m_m* data = (thread_data_m_m*)arg;
    uint64_t n = data->A->n;

    if(data->A->m != data->B->m ||data->A->m != data->C->m ||
        data->A->n != data->B->n || data->A->n != data->C->n){
            fprintf(stderr, "Erreur : les matrices doivent être de la même taille.\n");
            free_matrix(data->A);
            free_matrix(data->B);
            free_matrix(data->C);
            free(data);
            exit(EXIT_FAILURE);
        }

    for(uint64_t i = data->start_row; i < data->end_row; i++) {
        for(uint64_t j = 0; j < n; j++) {
            data->C->values[i][j] = data->A->values[i][j] - data->B->values[i][j];
        }
    }
    return NULL;

}

void* mult_m_m_thread(void* arg){

    thread_data_m_m* data = (thread_data_m_m*) arg;
    uint64_t n = data->A->n;
    uint64_t o = data->B->n;

    if (data->A->n != data->B->m || 
        data->A->m != data->C->m || data->B->n != data->C->n) {
        fprintf(stderr, "Erreur: Dimensions incompatibles A(%" PRIu64 " x %" PRIu64 ") et B(%" PRIu64 " x %" PRIu64 ")\n",
                data->A->m, data->A->n, data->B->m, data->B->n);
        exit(EXIT_FAILURE);
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
    return NULL; 

}

void* mult_m_v_thread(void* arg){
    
    thread_data_m_v* data = (thread_data_m_v*) arg;
    uint64_t n = data->A->n;
    if(data->A->n != data->B->m || data->A->m != data->C->m){
        fprintf(stderr, "Erreur: Dimensions incompatibles A(%" PRIu64 " x %" PRIu64 ") et B(%" PRIu64 ") C(%" PRIu64 ")\n",
                data->A->m, data->A->n, data->B->m, data->C->m);
        free_matrix(data->A);
        free_vector(data->B);
        free_vector(data->C);
        free(data);
        exit(EXIT_FAILURE);
    }

    for (uint64_t i = data->start_row; i < data->end_row; i++) {
        double res = 0;
        for (uint64_t j = 0; j < n; j++) {
            res += data->A->values[i][j] * data->B->values[j];
        }
        data->C->values[i] = res;
    }
    return NULL;
    
}

