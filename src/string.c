#include <string.h>
#include <stdio.h>
#include "string.h"

void print_string(const String *string) {
    for (size_t i = 0; i < string->len; i++) {
        putchar(string->data[i]);
    }
}

void print_string_wrapped(const String *string) {
    putchar('"');
    for (size_t i = 0; i < string->len; i++) {
        putchar(string->data[i]);
    }
    putchar('"');
}

String *heap_string_from(const char *c_str) {
    size_t len = strlen(c_str);
    char *str = malloc(len);
    strcpy(str, c_str);
    String *string = (String*)malloc(sizeof(String));
    string->data = str;
    string->len = len;
    string->cap = len;
    return string;
}

String string_from(const char *c_str) {
    size_t len = strlen(c_str);
    char *str = malloc(len);
    strcpy(str, c_str);
    String string = {
        .data = str,
        .len = len,
        .cap = len,
    };
    return string;
}

String *new_heap_string() {
    String *string = (String*)malloc(sizeof(String));
    string->data = NULL;
    string->len = 0;
    string->cap = 0;
    return string;
}

String new_string() {
    String string = {
        .data = NULL,
        .len = 0,
        .cap = 0,
    };
    return string;
}

void reallocate_string(String *string, size_t new_cap) {
    string->cap = new_cap;
    char *new_arr = malloc(string->cap);
    memcpy(new_arr, string->data, string->len);
    free(string->data);
    string->data = new_arr;
}

void push_char(String* string, char c) {
    if (string->len == string->cap) {
        size_t new_cap = (string->cap == 0) ? 1 : string->cap * 2;
        reallocate_string(string, new_cap);
    }
    string->data[string->len++] = c;
}

void push_string(String* string, const String *rhs) {
    if (string->cap < string->len + rhs->len) {
        size_t new_cap = string->len + rhs->len;
        reallocate_string(string, new_cap);
    }
    char *curr_end = string->data + string->len;
    memcpy(curr_end, rhs->data, rhs->len);
    string->len += rhs->len;
}
