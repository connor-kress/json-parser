#include <string.h>
#include "string.h"

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
