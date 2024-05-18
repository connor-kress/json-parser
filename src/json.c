#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "json.h"

void todo() {
    fprintf(stderr, "Not implemented yet");
    exit(1);
}

void reallocate_list(JsonList *list, size_t size) {
    list->cap = size;
    Json *new_arr = malloc(list->cap * sizeof(Json));
    for (int i = 0; i < list->len; i++) {
        new_arr[i] = list->data[i];
    }
    free(list->data);
    list->data = new_arr;
}

void reallocate_object(JsonObject *list, size_t size) {
    list->cap = size;
    KVPair *new_arr = malloc(list->cap * sizeof(KVPair));
    for (int i = 0; i < list->len; i++) {
        new_arr[i] = list->data[i];
    }
    free(list->data);
    list->data = new_arr;
}

void push_json(JsonList *list, Json val) {
    if (list->len == list->cap) {
        size_t new_cap = (list->cap == 0) ? 1 : list->cap * 2;
        reallocate_list(list, new_cap);
    }
    list->data[list->len] = val;
    list->len++;
}

void add_attr(JsonObject *obj, String key, Json val) {
    KVPair pair = {
        .key = key,
        .val = val,
    };
    if (obj->len == obj->cap) {
        size_t new_cap = (obj->cap == 0) ? 1 : obj->cap * 2;
        reallocate_object(obj, new_cap);
    }
    obj->data[obj->len] = pair;
    obj->len++;
}

void free_json(Json json) {
    if (json.type == JsonList_t) {
        JsonList *list = json.data;
        for (size_t i = 0; i < list->len; i++) {
            free_json(list->data[i]);
        }
    } else if (json.type == JsonObject_t) {
        JsonObject *obj = json.data;
        for (size_t i = 0; i < obj->len; i++) {
            KVPair *pair = &obj->data[i];
            free(pair->key.data);
            free_json(pair->val);
        }
    } else if (json.type == JsonString) {
        String *str = json.data;
        free(str->data);
    }
    free(json.data);
}

void print_string(const String *str) {
    for (size_t i = 0; i < str->len; i++) {
        putchar(str->data[i]);
    }
}

void print_string_wrapped(const String *str) {
    putchar('"');
    for (size_t i = 0; i < str->len; i++) {
        putchar(str->data[i]);
    }
    putchar('"');
}
void print_json(const Json *json) {
    if (json->type == JsonInt) {
        printf("%d", *(int*)json->data);
    } else if (json->type == JsonFloat) {
        printf("%f", *(float*)json->data);
    } else if (json->type == JsonList_t) {
        JsonList *list = json->data;
        putchar('[');
        for (size_t i = 0; i < list->len; i++) {
            print_json(&list->data[i]);
            if (i != list->len - 1) printf(", ");
        }
        putchar(']');
    } else if (json->type == JsonObject_t) {
        JsonObject *obj = json->data;
        putchar('{');
        for (size_t i = 0; i < obj->len; i++) {
            KVPair *pair = &obj->data[i];
            print_string_wrapped(&pair->key);
            printf(": ");
            print_json(&pair->val);
            if (i != obj->len - 1) printf(", ");
        }
        putchar('}');
    } else if (json->type == JsonString) {
        print_string_wrapped((String*)json->data);
    } else if (json->type == JsonBool) {
        bool *val = (bool*)json->data;
        printf(*val ? "true" : "false");
    } else if (json->type == JsonNull) {
        printf("null");
    } else {
        fprintf(stderr, "Nonexistent json type");
        exit(1);
    }
}

Json int_from(int val) {
    int *data = malloc(sizeof(int));
    *data = val;
    Json json = {
        .type = JsonInt,
        .data = data,
    };
    return json;
}

Json float_from(float val) {
    int *data = malloc(sizeof(float));
    *data = val;
    Json json = {
        .type = JsonFloat,
        .data = data,
    };
    return json;
}

Json bool_from(bool val) {
    int *data = malloc(sizeof(bool));
    *data = val;
    Json json = {
        .type = JsonBool,
        .data = data,
    };
    return json;
}

Json new_null() {
    Json json = {
        .type = JsonNull,
        .data = NULL,
    };
    return json;
}

Json json_string_from(String* string) {
    Json json = {
        .type = JsonString,
        .data = string
    };
    return json;
}

Json json_string_from_cstr(const char *c_str) {
    String *string = heap_string_from(c_str);
    return json_string_from(string);
}

Json new_list() {
    JsonList *list = malloc(sizeof(JsonList));
    list->data = NULL;
    list->len = 0;
    list->cap = 0;
    Json json = {
        .type = JsonList_t,
        .data = list,
    };
    return json;
}

Json new_object() {
    JsonObject *obj = malloc(sizeof(JsonObject));
    obj->data = NULL;
    obj->len = 0;
    obj->cap = 0;
    Json json = {
        .type = JsonObject_t,
        .data = obj,
    };
    return json;
}
