#include "vector.h"

Vector * vector_new(uint32_t item_size, uint32_t intitial_capacity) {
    if (intitial_capacity == 0)
        return NULL;

    Vector *v = (Vector *) malloc(sizeof(Vector));

    if (!v)
        return NULL;

    v->item_size = item_size;
    v->capacity = intitial_capacity;
    v->items_count = 0;
    v->data = malloc(item_size * intitial_capacity);

    if (!v->data)
    {
        free(v);
        return NULL;
    }

    return v;
}

void vector_multiply(Vector *v) {
    v->capacity = v->capacity * VECTOR_MULTIPLIER;
    v->data = realloc(v->data, v->capacity);
}

void vector_push_back(Vector *v, void *item) {
    if (v->items_count >= v->capacity)
        vector_multiply(v);
    memcpy((char *) v->data + v->items_count * v->item_size, item, v->item_size);
    v->items_count++;
}

void * vector_get(Vector *v, uint32_t idx) {
    if (idx >= v->items_count)
        return NULL;
    
    return (void *) ((char *) v->data + idx * v->item_size);
}