#include <stdbool.h>
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
    char *str = (char*)malloc(len + 1);
    memcpy(str, c_str, len);
    str[len] = '\0';
    String *string = (String*)malloc(sizeof(String));
    string->data = str;
    string->len = len;
    string->cap = len;
    return string;
}

String string_from(const char *c_str) {
    size_t len = strlen(c_str);
    char *str = (char*)malloc(len + 1);
    memcpy(str, c_str, len);
    str[len] = '\0';
    String string = {
        .data = str,
        .len = len,
        .cap = len,
    };
    return string;
}

String *new_heap_string() {
    String *string = (String*)malloc(sizeof(String));
    string->data = (char*)malloc(1);
    string->data[0] = '\0';
    string->len = 0;
    string->cap = 0;
    return string;
}

String new_string() {
    String string = {
        .data = (char*)malloc(1),
        .len = 0,
        .cap = 0,
    };
    string.data[0] = '\0';
    return string;
}

void reallocate_string(String *string, size_t new_cap) {
    string->cap = new_cap;
    char *new_arr = (char*)malloc(string->cap + 1);
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
    string->data[string->len] = '\0';
}

void push_string(String* string, const String *rhs) {
    if (string->cap < string->len + rhs->len) {
        size_t new_cap = string->len + rhs->len;
        reallocate_string(string, new_cap);
    }
    char *curr_end = string->data + string->len;
    memcpy(curr_end, rhs->data, rhs->len);
    string->len += rhs->len;
    string->data[string->len] = '\0';
}

bool string_eq(const String *lhs, const String *rhs) {
    if (lhs->len != rhs->len) return false;
    for (size_t i = 0; i < lhs->len; i++) {
        if (lhs->data[i] != rhs->data[i])
            return false;
    }
    return true;
}

bool string_eq_cstr(const String *lhs, const char *rhs){
    if (lhs->len != strlen(rhs)) return false;
    for (size_t i = 0; i < lhs->len; i++) {
        if (lhs->data[i] != rhs[i])
            return false;
    }
    return true;
}
