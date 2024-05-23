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

void print_string_wrapped(const String *string);

bool string_eq(const String *lhs, const String *rhs);

bool string_eq_cstr(const String *lhs, const char *rhs);

String copy_string(const String *str);

String *copy_heap_string(const String *str);

#endif // MY_STRING_H_
