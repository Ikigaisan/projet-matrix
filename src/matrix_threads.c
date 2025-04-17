#include "../headers/matrix.h"
#include "../headers/vector.h"
#include "../headers/matrix_threads.h"

// Define thread_data_mult if not already defined
typedef struct {
    matrix* A;
    vector* v;
    vector* result;
    uint64_t start_row;
    uint64_t end_row;
} thread_data_mult;
#include <float.h>
#include <math.h>
#include <inttypes.h>

// Define thread_data_mult_m if not already defined
typedef struct {
    matrix* A;
    matrix* B;
    matrix* result;
    uint64_t start_row;
    uint64_t end_row;
} thread_data_mult_m;

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
void* mult_m_v_thread(void* arg) {
    thread_data_mult* data = (thread_data_mult*)arg;
    matrix* A = data->A;
    vector* B = data->v;
    vector* C = data->result;
    uint64_t m = A->m;
    uint64_t n = A->n;

    for (uint64_t i = data->start_row; i < data->end_row; i++) {
        C->values[i] = 0;
        for (uint64_t j = 0; j < n; j++) {
            C->values[i] += A->values[i][j] * B->values[j];
        }
    }
    return NULL;
}


void* mult_m_m_thread(void* arg) {
    thread_data_mult_m* data = (thread_data_mult_m*)arg;
    matrix* A = data->A;
    matrix* B = data->B;
    matrix* C = data->result;
    uint64_t m = A->m;
    uint64_t n = A->n;
    uint64_t p = B->n;

    for (uint64_t i = data->start_row; i < data->end_row; i++) {
        for (uint64_t j = 0; j < p; j++) {
            C->values[i][j] = 0;
            for (uint64_t k = 0; k < n; k++) {
                C->values[i][j] += A->values[i][k] * B->values[k][j];
            }
        }
    }
    return NULL;
}