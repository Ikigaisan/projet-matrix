#include "../headers/matrix.h"
#include "../headers/vector.h"
#include "../headers/matrix_threads.h"
#include <float.h>
#include <math.h>
#include <inttypes.h>

void* transp_thread(void* arg) {
    
    thread_data_transp* data = (thread_data_transp*)arg;
    uint64_t m = data->A->m;
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
    uint64_t m = data->A->m;
    uint64_t n = data->A->n;

    for(uint64_t i = data->start_row; i < data->end_row; i++) {
        for(uint64_t j = 0; j < n; j++) {
            data->C->values[i][j] = data->A->values[i][j] + data->B->values[i][j];
        }
    }
    return NULL;

}

