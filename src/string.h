#ifndef MY_STRING_H_
#define MY_STRING_H_

#include <stdlib.h>

typedef struct {
    char *data;
    size_t len, cap;
} String;

void debug_string(const String *str);

String string_from(const char *c_str);

String *heap_string_from(const char *c_str);

String *new_heap_string();

String new_string();

void push_char(String* buf, char c);

void push_string(String* buf, const String *str);

void write_string_repr(String *buf, const String *str);

bool string_eq(const String *lhs, const String *rhs);

bool string_eq_cstr(const String *lhs, const char *rhs);

String copy_string(const String *str);

String *copy_heap_string(const String *str);

void write_format(String *buf, const char *format, ...);

#endif // MY_STRING_H_
