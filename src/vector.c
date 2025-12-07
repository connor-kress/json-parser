#include <stdlib.h>
#include <string.h>
#include "vector.h"

void vec_init(Vec *vec, size_t item_size) {
    vec->data = NULL;
    vec->len = 0;
    vec->cap = 0;
    vec->item_size = item_size;
}

Vec new_vec(size_t item_size) {
    Vec vec;
    vec_init(&vec, item_size);
    return vec;
}

Vec *new_heap_vec(size_t item_size) {
    Vec *vec = malloc(sizeof(Vec));
    vec_init(vec, item_size);
    return vec;
}

void reallocate_vec(Vec *vec, size_t new_cap) {
    vec->cap = new_cap;
    void *new_arr = malloc(vec->cap * vec->item_size);
    memcpy(new_arr, vec->data, vec->len * vec->item_size);
    free(vec->data);
    vec->data = new_arr;
}

void *vec_get(const Vec *vec, size_t idx) {
    return vec->data + (idx*vec->item_size);
}

void vec_set(Vec *vec, size_t idx, const void *val) {
    void *arr_ptr = vec->data + (idx*vec->item_size);
    memcpy(arr_ptr, val, vec->item_size);
}

void vec_push(Vec *vec, const void* val) {
    if (vec->len == vec->cap) {
        size_t new_cap = (vec->cap == 0) ? 1 : vec->cap * 2;
        reallocate_vec(vec, new_cap);
    }
    vec_set(vec, vec->len, val);
    vec->len++;
}
