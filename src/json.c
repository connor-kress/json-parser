#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "json.h"
#include "vector.h"

void TODO() {
    fprintf(stderr, "Not implemented yet");
    exit(1);
}

const char *get_json_type_str(const JsonType *type) {
    switch (*type) {
        case JsonObject_t: return "JsonObject_t";
        case JsonList_t:   return "JsonList_t";
        case JsonString:   return "JsonString";
        case JsonInt:      return "JsonInt";
        case JsonFloat:    return "JsonFloat";
        case JsonBool:     return "JsonBool";
        case JsonNull:     return "JsonNull";
        default:           return "UNKNOWN_TYPE";
    };
}

void push_json(Vec *list, Json val) {
    vec_push(list, &val);
}

void add_attr(Vec *obj, String key, Json val) {
    KVPair pair = {
        .key = key,
        .val = val,
    };
    vec_push(obj, &pair);
}

void free_json(Json *json) {
    if (json->type == JsonList_t) {
        Vec *list = json->data;
        for (size_t i = 0; i < list->len; i++) {
            Json *item = (Json*)vec_get(list, i);
            free_json(item);
        }
        free(list->data);
    } else if (json->type == JsonObject_t) {
        Vec *obj = (Vec*)json->data;
        for (size_t i = 0; i < obj->len; i++) {
            KVPair *pair = (KVPair*)vec_get(obj, i);
            free(pair->key.data);
            free_json(&pair->val);
        }
        free(obj->data);
    } else if (json->type == JsonString) {
        String *str = (String*)json->data;
        free(str->data);
    }
    free(json->data);
}

void write_json(String *buf, const Json *json) {
    if (json->type == JsonInt) {
        write_format(buf, "%d", *(int*)json->data);
    } else if (json->type == JsonFloat) {
        write_format(buf, "%f", *(float*)json->data);
    } else if (json->type == JsonList_t) {
        const Vec *list = json->data;
        push_char(buf, '[');
        for (size_t i = 0; i < list->len; i++) {
            const Json *item = vec_get(list, i);
            write_json(buf, item);
            if (i != list->len - 1) write_format(buf, ", ");
        }
        push_char(buf, ']');
    } else if (json->type == JsonObject_t) {
        Vec *obj = json->data;
        push_char(buf, '{');
        for (size_t i = 0; i < obj->len; i++) {
            const KVPair *pair = vec_get(obj, i);

            write_string_repr(buf, &pair->key);
            write_format(buf, ": ");
            write_json(buf, &pair->val);
            if (i != obj->len - 1) write_format(buf, ", ");
        }
        push_char(buf, '}');
    } else if (json->type == JsonString) {
        const String *str = json->data;
        write_string_repr(buf, str);
    } else if (json->type == JsonBool) {
        bool val = *(bool*)json->data;
        write_format(buf, val ? "true" : "false");
    } else if (json->type == JsonNull) {
        write_format(buf, "null");
    } else {
        fprintf(stderr, "Nonexistent json type");
        exit(1);
    }
}

String stringify(const Json *json) {
    String buf = new_string();
    write_json(&buf, json);
    return buf;
}

Json int_from(int val) {
    int *data = malloc(sizeof(int));
    *data = val;
    return (Json) {
        .type = JsonInt,
        .data = (void*)data,
    };
}

Json float_from(float val) {
    float *data = malloc(sizeof(float));
    *data = val;
    return (Json) {
        .type = JsonFloat,
        .data = data,
    };
}

Json bool_from(bool val) {
    bool *data = malloc(sizeof(bool));
    *data = val;
    return (Json) {
        .type = JsonBool,
        .data = data,
    };
}

Json new_null() {
    return (Json) {
        .type = JsonNull,
        .data = NULL,
    };
}

Json json_string_from(String* string) {
    return (Json) {
        .type = JsonString,
        .data = string,
    };
}

Json json_string_from_cstr(const char *c_str) {
    String *string = heap_string_from(c_str);
    return json_string_from(string);
}

Json new_list() {
    return (Json) {
        .type = JsonList_t,
        .data = new_heap_vec(sizeof(Json)),
    };
}

Json new_object() {
    return (Json) {
        .type = JsonObject_t,
        .data = new_heap_vec(sizeof(KVPair)),
    };
}
