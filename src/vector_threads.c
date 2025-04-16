#include "../headers/vector.h"
#include "../headers/vector_threads.h"
#include <math.h>
#include <inttypes.h>



void* add_v_v_thread(void *arg) {
    thread_data_v_v *data = (thread_data_v_v *)arg;  // On "déballe" le arg

    for (uint64_t i = data->start_idx; i < data->end_idx; i++) {
        data->z->values[i] = data->x->values[i] + data->y->values[i];
    }

    return NULL;
}

void* sub_v_v_thread (void *arg) {
    thread_data_v_v *data = (thread_data_v_v *)arg;

    for(uint64_t i = data->start_idx; i < data->end_idx; i++) {
        data->z->values[i] = data->x->values[i] - data->y->values[i];
    }

    return NULL; 

}

