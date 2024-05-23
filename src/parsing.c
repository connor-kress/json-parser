#include <ctype.h>
#include <stdbool.h>
#include <stdlib.h>
#include "string.h"
#include "json.h"
#include "vector.h"
#include "parsing.h"

// typedef enum {
//     Numeric_t,
//     String_t,
//     Keyword,
//     Comma,
//     Lbracket,
//     Rbracket,
//     Lcurly,
//     Rcurly,
// } TokenType;

static char SINGLE_CHAR_TOKENS[] = {',', ':', '[', ']', '{', '}'};
static TokenType SINGLE_CHAR_TOKEN_MAPPINGS[] =
    {Comma, Colon, Lbracket, Rbracket, Lcurly, Rcurly};

void free_token_vec(Vec *tokens) {
    for (size_t i = 0; i < tokens->len; i++) {
        Token* t = (Token*)vec_get(tokens, i);
        if (t->data != NULL) {
            free(t->data->data);
            free(t->data);
        }
    }
}

void print_token_vec(const Vec *vec) {
    char* type_name = "ERROR";
    printf("Tokens = [\n");
    for (size_t i = 0; i < vec->len; i++) {
        Token* t = (Token*)vec_get(vec, i);
        if (t->type == Numeric_t) {
            type_name = "Numeric_t";
        } else if (t->type == String_t) {
            type_name = "String_t";
        } else if (t->type == Keyword) {
            type_name = "Keyword";
        } else if (t->type == Comma) {
            type_name = "Comma";
        } else if (t->type == Colon) {
            type_name = "Colon";
        } else if (t->type == Lcurly) {
            type_name = "Lcurly";
        } else if (t->type == Rcurly) {
            type_name = "Rcurly";
        } else if (t->type == Lbracket) {
            type_name = "Lbracket";
        } else if (t->type == Rbracket) {
            type_name = "Rbracket";
        }
        if (t->data == NULL) {
            printf("\tToken(%s, NULL),\n", type_name);
        } else {
            printf("\tToken(%s, \"%s\"),\n", type_name, t->data->data);
        }
    }
    printf("]\n");
}

String *read_string_literal(const String* string, size_t *i) {
    (*i)++; // first quotation
    String *buf = new_heap_string();
    for (; *i < string->len; (*i)++) {
        char c = string->data[*i];
        if (c == '"') { // second quotation
            (*i)++;
            break;
        }
        push_char(buf, c);
    }
    if (*i >= string->len) {
        fprintf(stderr, "Error: Failed to close string literal");
        free(buf->data); free(buf);
        return NULL;
    };
    return buf;
}
String *read_numeric_literal(const String* string, size_t *i) {
    String *buf = new_heap_string();
    for (; *i < string->len; (*i)++) {
        char c = string->data[*i];
        if (!isdigit(c) && c != '.') {
            break;
        }
        push_char(buf, c);
    }
    return buf;
}

String *read_keyword(const String* string, size_t *i) {
    String *buf = new_heap_string();
    for (; *i < string->len; (*i)++) {
        char c = string->data[*i];
        if (!isalpha(c)) {
            break;
        }
        push_char(buf, c);
    }
    return buf;
}

Tokens tokenize_string(const String* string, bool *did_error) {
    Tokens tokens_obj;
    if (string->len == 0) {
        fprintf(stderr, "Error: Cannot tokenize empty string");
        *did_error = true;
        return tokens_obj;
    }
    Vec tokens = new_vec(sizeof(Token));
    size_t i = 0;
    for (;;) {
        if (i >= string->len) break;
        while (isspace(string->data[i])) { i++; }
        char c = string->data[i];
        size_t single_char_token_count = sizeof(SINGLE_CHAR_TOKENS)
                                       / sizeof(SINGLE_CHAR_TOKENS[0]);
        bool found_token = false;
        for (size_t c_i = 0; c_i < single_char_token_count; c_i++) {
            if (c != SINGLE_CHAR_TOKENS[c_i]){
                // printf("char(%c) != char(%c)\n", c, SINGLE_CHAR_TOKENS[c_i]);
                continue;
            }
            // printf("Found single char token: `%c`\n", c);
            // printf("c_i = %lu\n", c_i);
            Token t = {
                .type = SINGLE_CHAR_TOKEN_MAPPINGS[c_i],
                .data = NULL,
            };
            vec_push(&tokens, &t);
            found_token = true;
            i++;
        }
        if (found_token) continue;
        if (c == '"') { // string literal
            String *string_data = read_string_literal(string, &i);
            // printf("Found string literal: \"%s\"\n", string_data->data);
            if (string_data == NULL) {
                free_token_vec(&tokens);
                *did_error = true;
                return tokens_obj;
            }
            Token t = {
                .type = String_t,
                .data = string_data,
            };
            vec_push(&tokens, &t);
        } else if (isdigit(c)) { // numeric literal
            Token t = {
                .type = Numeric_t,
                .data = read_numeric_literal(string, &i),
            };
            // printf("Found numeric literal: \"%s\"\n", t.data->data);
            vec_push(&tokens, &t);
        } else if (isalpha(c)) { // keyword (null, true, false)
            Token t = {
                .type = Keyword,
                .data = read_keyword(string, &i),
            };
            // printf("Found keyword: \"%s\"\n", t.data->data);
            vec_push(&tokens, &t);
        } else {
            fprintf(stderr, "Error: Invalid token '%c'", c);
            free_token_vec(&tokens);
            *did_error = true;
            return tokens_obj;
        }
    }

    tokens_obj.data = tokens;
    tokens_obj.curr = 0;
    return tokens_obj;
}

Json parse_json(const String *string, bool *did_error) {
    bool token_err = false;
    Tokens tokens = tokenize_string(string, &token_err);
    if (token_err) {
        *did_error = true;
        return new_null();
    }
    print_token_vec(&tokens.data);
    free_token_vec(&tokens.data);
    return new_null();
}
