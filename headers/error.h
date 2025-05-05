#ifndef ERRORS_H
#define ERRORS_H

#define ERROR_NULL_POINTER -1
#define ERROR_SIZE_MISMATCH -2
#define ERROR_ALLOC_STRUCT -3
#define ERROR_ALLOC_VALUES -4
#define ERROR_NULL_VALUES -5
#define ERROR_INDEX_OUT_OF_BOUNDS -6
#define ERROR_THREADS -7
#define ERROR_NO_SOLUTION -8
#define ERROR_DIV_bY_0 -9
#define ERROR_FILE -10

void handle_error(int code);

#endif
