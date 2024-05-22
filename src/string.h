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

void push_char(String* string, char c);

void push_string(String* string, const String *rhs);

void print_string(const String *string);

void print_string_wrapped(const String *string);

#endif // MY_STRING_H_
