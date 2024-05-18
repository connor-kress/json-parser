#ifndef JSON_H_
#define JSON_H_

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "string.h"

typedef enum {
    JsonObject_t,
    JsonList_t,
    JsonString,
    JsonInt,
    JsonFloat,
    JsonBool,
    JsonNull,
} JsonType;

typedef struct {
    JsonType type;
    void *data;
} Json;

typedef struct {
    Json *data;
    size_t len, cap;
} JsonList;

typedef struct {
    String key;
    Json val;
} KVPair;

typedef struct {
    KVPair *data;
    size_t len, cap;
} JsonObject;

void reallocate_list(JsonList *list, size_t size);

void reallocate_object(JsonObject *list, size_t size);

void push_json(JsonList *list, Json val);

void add_attr(JsonObject *obj, String key, Json val);

void free_json(Json json);

void print_string(const String *str);

void print_json(const Json *json);

Json int_from(int val);

Json float_from(float val);

Json bool_from(bool val);

Json new_null();

Json json_string_from(String* string);

Json json_string_from_cstr(const char *c_str);

Json new_list();

Json new_object();

#endif // JSON_H_
