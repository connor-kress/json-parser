#ifndef VECTOR_H_
#define VECTOR_H_

#include <stdlib.h>

typedef struct {
    void *data;
    size_t len, cap, item_size;
} Vec;

void vec_init(Vec *vec, size_t item_size);

Vec new_vec(size_t item_size);

Vec *new_heap_vec(size_t item_size);

void *vec_get(const Vec *vec, size_t idx);

void vec_set(Vec *vec, size_t idx, const void *val);

void vec_push(Vec *vec, const void* val);

#endif // VECTOR_H_
