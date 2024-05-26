#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "json.h"
#include "vector.h"

void todo() {
    fprintf(stderr, "Not implemented yet");
    exit(1);
}

// typedef enum {
//     JsonObject_t,
//     JsonList_t,
//     JsonString,
//     JsonInt,
//     JsonFloat,
//     JsonBool,
//     JsonNull,
// } JsonType;

const char *get_json_type_str(const JsonType *type) {
    if (*type == JsonObject_t) {
        return "JsonObject_t";
    } else if (*type == JsonList_t) {
        return "JsonList_t";
    } else if (*type == JsonString) {
        return "JsonString";
    } else if (*type == JsonInt) {
        return "JsonInt";
    } else if (*type == JsonFloat) {
        return "JsonFloat";
    } else if (*type == JsonBool) {
        return "JsonBool";
    } else if (*type == JsonNull) {
        return "JsonNull";
    } else {
        return "UNKNOWN JSON TYPE";
    }
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
    int *data = (int*)malloc(sizeof(int));
    *data = val;
    Json json = {
        .type = JsonInt,
        .data = (void*)data,
    };
    return json;
}

Json float_from(float val) {
    float *data = (float*)malloc(sizeof(float));
    *data = val;
    Json json = {
        .type = JsonFloat,
        .data = (void*)data,
    };
    return json;
}

Json bool_from(bool val) {
    bool *data = (bool*)malloc(sizeof(bool));
    *data = val;
    Json json = {
        .type = JsonBool,
        .data = (void*)data,
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
        .data = (void*)string,
    };
    return json;
}

Json json_string_from_cstr(const char *c_str) {
    String *string = heap_string_from(c_str);
    return json_string_from(string);
}

Json new_list() {
    Json json = {
        .type = JsonList_t,
        .data = (void*)new_heap_vec(sizeof(Json)),
    };
    return json;
}

Json new_object() {
    Json json = {
        .type = JsonObject_t,
        .data = (void*)new_heap_vec(sizeof(KVPair)),
    };
    return json;
}
