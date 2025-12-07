#include <stdarg.h>
#include <stdbool.h>
#include <string.h>
#include <stdio.h>
#include "string.h"

void debug_string(const String *string) {
    printf("String {\n");
    printf("    data: \"%s\",\n", string->data);
    printf("    len: %lu,\n", string->len);
    printf("    cap: %lu,\n", string->cap);
    printf("}\n");
}

// print escaped characters correctly
void write_string_repr(String *buf, const String *str) {
    push_char(buf, '"');
    push_string(buf, str);
    push_char(buf, '"');
}

void copy_cstr_to_string(String *string, const char *c_str) {
    if (string->data != NULL) {
        free(string->data);
    }
    size_t len = strlen(c_str);
    reallocate_string(string, len);
    memcpy(string->data, c_str, len);
    string->data[len] = '\0';
    string->len = len;
}

String string_from(const char *c_str) {
    String string;
    copy_cstr_to_string(&string, c_str);
    return string;
}

String *heap_string_from(const char *c_str) {
    String *string = malloc(sizeof(String));
    copy_cstr_to_string(string, c_str);
    return string;
}

void string_init(String *string) {
    string->data = malloc(1);
    string->data[0] = '\0';
    string->len = 0;
    string->cap = 0;
}

String *new_heap_string() {
    String *string = malloc(sizeof(String));
    string_init(string);
    return string;
}

String new_string() {
    String string;
    string_init(&string);
    return string;
}

void reallocate_string(String *string, size_t new_cap) {
    string->cap = new_cap;
    char *new_arr = malloc(string->cap + 1);
    if (new_arr == NULL) {
        fprintf(stderr, "Error allocating new string buffer");
        exit(1);
    }
    memcpy(new_arr, string->data, string->len + 1);
    free(string->data);
    string->data = new_arr;
}

void push_char(String* buf, char c) {
    if (buf->len == buf->cap) {
        size_t new_cap = (buf->cap == 0) ? 1 : buf->cap * 2;
        reallocate_string(buf, new_cap);
    }
    buf->data[buf->len++] = c;
    buf->data[buf->len] = '\0';
}

void push_string(String* buf, const String *str) {
    if (buf->cap < buf->len + str->len) {
        size_t new_cap = buf->len + str->len;
        reallocate_string(buf, new_cap);
    }
    char *curr_end = buf->data + buf->len;
    memcpy(curr_end, str->data, str->len);
    buf->len += str->len;
    buf->data[buf->len] = '\0';
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

String copy_string(const String *str) {
    String new_str;
    new_str.data = (char*)malloc(str->cap + 1);
    memcpy(new_str.data, str->data, str->cap + 1);
    new_str.cap = str->cap;
    new_str.len = str->len;
    return new_str;
}

String *copy_heap_string(const String *str) {
    String *new_str = (String*)malloc(sizeof(String));
    new_str->data = (char*)malloc(str->cap + 1);
    memcpy(new_str->data, str->data, str->cap + 1);
    new_str->cap = str->cap;
    new_str->len = str->len;
    return new_str;
}

void write_format(String *buf, const char *format, ...) {
    va_list args;
    va_start(args, format);
    int size = vsnprintf(NULL, 0, format, args);
    if (size < 0) {
        va_end(args);
        fprintf(stderr, "Invalid format specified\n");
        exit(1);
    }
    reallocate_string(buf, buf->cap + size);
    char *end_ptr = buf->data + buf->len;
    va_start(args, format);
    int chars_written = vsnprintf(end_ptr, size + 1, format, args);
    if (chars_written  < 0) {
        va_end(args);
        fprintf(stderr, "Error while writing to buffer\n");
        exit(1);
    }
    if (size != chars_written) {
        fprintf(stderr, "Unexpected size and chars_written mismatch\n");
    }
    buf->len += chars_written;
    va_end(args);
}
