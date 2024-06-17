#ifndef _VECTOR_H
#define _VECTOR_H

#include <stdint.h>
#include <stdlib.h>
#include <memory.h>

#define VECTOR_MULTIPLIER 2

typedef struct Vector {
    void *data;
    uint32_t item_size;
    uint32_t items_count;
    uint32_t capacity;
} Vector;


Vector * vector_new(uint32_t item_size, uint32_t intitial_capacity);
void vector_multiply(Vector *v);
void vector_push_back(Vector *v, void *item);
void * vector_get(Vector *v, uint32_t idx);

#endif