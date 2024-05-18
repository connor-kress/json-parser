#ifndef MY_STRING_H_
#define MY_STRING_H_

#include <stdlib.h>

typedef struct {
    char *data;
    size_t len, cap;
} String;

String string_from(const char *c_str);

String *heap_string_from(const char *c_str);

String *new_heap_string();

String new_string();

#endif // MY_STRING_H_
