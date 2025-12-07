#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include "string.h"
#include "json.h"
#include "vector.h"
#include "parsing.h"

static char SINGLE_CHAR_TOKENS[] = {',', ':', '[', ']', '{', '}'};
static TokenType SINGLE_CHAR_TOKEN_MAPPINGS[] =
    {Comma, Colon, Lbracket, Rbracket, Lcurly, Rcurly};

void free_token_vec(Vec *tokens) {
    for (size_t i = 0; i < tokens->len; i++) {
        Token* t = vec_get(tokens, i);
        if (t->data != NULL) {
            free(t->data->data);
            free(t->data);
        }
    }
    free(tokens->data);
}

const char *get_token_type_str(const TokenType *type) {
        if (*type == Numeric_t) {
            return "Numeric_t";
        } else if (*type == String_t) {
            return "String_t";
        } else if (*type == Keyword) {
            return "Keyword";
        } else if (*type == Comma) {
            return "Comma";
        } else if (*type == Colon) {
            return "Colon";
        } else if (*type == Lcurly) {
            return "Lcurly";
        } else if (*type == Rcurly) {
            return "Rcurly";
        } else if (*type == Lbracket) {
            return "Lbracket";
        } else if (*type == Rbracket) {
            return "Rbracket";
        } else {
            return "UNKNOWN TOKEN TYPE";
        }
}

void print_token_vec(const Vec *vec) {
    printf("Tokens = [\n");
    for (size_t i = 0; i < vec->len; i++) {
        Token* t = vec_get(vec, i);
        const char *type_name = get_token_type_str(&t->type);
        if (t->data == NULL) {
            printf("\tToken(%s, NULL),\n", type_name);
        } else {
            printf("\tToken(%s, \"%s\"),\n", type_name, t->data->data);
        }
    }
    printf("]\n");
}

// read escaped characters correctly
String *read_string_literal(const String* string, size_t *i) {
    String *buf = new_heap_string();
    (*i)++; // first quotation
    for (; *i < string->len; (*i)++) {
        char c = string->data[*i];
        if (c == '"') { // second quotation
            (*i)++;
            return buf;
        }
        push_char(buf, c);
    }
    fprintf(stderr, "Error: Failed to close string literal\n");
    free(buf->data); free(buf);
    return NULL;
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
    Tokens tokens_obj = { .data = { .item_size = sizeof(Token) } };
    if (string->len == 0) {
        fprintf(stderr, "Error: Cannot tokenize empty string\n");
        *did_error = true;
        return tokens_obj;
    }
    Vec tokens = new_vec(sizeof(Token));
    size_t i = 0;
    for (;;) {
        while ( i < string->len && isspace(string->data[i])) { i++; }
        if (i >= string->len) break;
        char c = string->data[i];
        size_t single_char_token_count = sizeof(SINGLE_CHAR_TOKENS)
                                       / sizeof(SINGLE_CHAR_TOKENS[0]);
        bool found_token = false;
        for (size_t c_i = 0; c_i < single_char_token_count; c_i++) {
            if (c != SINGLE_CHAR_TOKENS[c_i]){
                continue;
            }
            // printf("Found single char token: `%c`\n", c);
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
            fprintf(stderr, "Error: Invalid token '%c' (%d)\n", c, c);
            free_token_vec(&tokens);
            *did_error = true;
            return tokens_obj;
        }
    }

    tokens_obj.data = tokens;
    tokens_obj.curr = 0;
    return tokens_obj;
}

bool remaining(const Tokens *tokens) {
    return tokens->curr < tokens->data.len;
}

const Token *peek_token(const Tokens *tokens) {
    if (!remaining(tokens)) {
        fprintf(stderr, "Called peek with no remaining tokens\n");
        return NULL;
    }
    return (const Token*)vec_get(&tokens->data, tokens->curr);
}

const Token *next_token(Tokens *tokens) {
    if (!remaining(tokens)) {
        fprintf(stderr, "Called next with no remaining tokens: %lu < %lu\n", tokens->curr, tokens->data.len);
        return NULL;
    }
    const Token *token = (const Token*)vec_get(&tokens->data, tokens->curr);
    // printf("i: %lu -> %lu\n", tokens->curr, tokens->curr + 1);
    tokens->curr++;
    return token;
}

Json parse_numeric_literal(const Token *token, bool *did_error) {
    // printf("called parse_numeric_literal\n");
    const String *num_str = token->data;
    bool is_float = false;
    for (size_t i = 0; i < num_str->len; i++) {
        char c = num_str->data[i];
        if (c == '.' && !is_float) {
            is_float = true;
        } else if (!isdigit(c)) {
            fprintf(stderr, "Error: Invalid literal `%s`\n", num_str->data);
            *did_error = true;
            return new_null();
        }
    }
    if (is_float) {
        float val = atof(num_str->data);
        return float_from(val);
    } else {
        int val = atoi(num_str->data);
        return int_from(val);
    }
}

Json parse_keyword(const Token *token, bool *did_error) {
    // printf("called parse_keyword\n");
    if (string_eq_cstr(token->data, "null")) {
        return new_null();
    } else if (string_eq_cstr(token->data, "true")) {
        return bool_from(true);
    } else if (string_eq_cstr(token->data, "false")) {
        return bool_from(false);
    } else {
        fprintf(stderr, "Error: Invalid keyword `%s`\n", token->data->data);
        *did_error = true;
        return new_null();
    }
}

Json parse_json_from_tokens(Tokens *tokens, bool *did_error);

Json parse_json_object(Tokens *tokens, bool *did_error) {
    // printf("called parse_json_object\n");
    Vec *pairs = new_heap_vec(sizeof(KVPair));
    Json json = {
        .type = JsonObject_t,
        .data = pairs,
    };
    next_token(tokens); // skip '{'
    bool recursive_error = false;
    for (;;) {
        Json key = parse_json_from_tokens(tokens, &recursive_error);
        if (recursive_error) {
            free_json(&json);
            *did_error = true;
            return new_null();
        }
        if (key.type != JsonString) {
            free_json(&json);
            fprintf(stderr, "Error: Expected object key but found `%s` instead\n",
                    get_json_type_str(&key.type));
            *did_error = true;
            return new_null();
        }
        const Token *kv_sep = next_token(tokens);
        if (kv_sep == NULL) {
            free_json(&json);
            fprintf(stderr, "Error: Unfinished object literal\n");
            *did_error = true;
            return new_null();
        }
        if (kv_sep->type != Colon) {
            free_json(&json);
            fprintf(stderr, "Error: Expected colon but found `%s` instead\n",
                    get_token_type_str(&kv_sep->type));
            *did_error = true;
            return new_null();
        }
        Json item = parse_json_from_tokens(tokens, &recursive_error);
        if (recursive_error) {
            free_json(&json);
            *did_error = true;
            return new_null();
        }
        add_attr(pairs, *(String*)key.data, item);
        const Token *delim = next_token(tokens);
        if (delim == NULL) {
            free_json(&json);
            fprintf(stderr, "Error: Failed to close object literal\n");
            *did_error = true;
            return new_null();
        }
        if (delim->type == Comma) {
            continue;
        } else if (delim->type == Rcurly) {
            break;
        } else {
            free_json(&json);
            fprintf(stderr, "Error: Unexpected token of type `%s` in object\n",
                    get_token_type_str(&delim->type));
            *did_error = true;
            return new_null();
        }
    }
    return json;
}

Json parse_json_list(Tokens *tokens, bool *did_error) {
    // printf("called parse_json_list\n");
    Vec *vec = new_heap_vec(sizeof(Json));
    Json json = {
        .type = JsonList_t,
        .data = vec,
    };
    next_token(tokens); // skip '['
    bool recursive_error = false;
    for (;;) {
        Json item = parse_json_from_tokens(tokens, &recursive_error);
        if (recursive_error) {
            free_json(&json);
            *did_error = true;
            return new_null();
        }
        vec_push(vec, &item);
        const Token *delim = next_token(tokens);
        if (delim == NULL) {
            free_json(&json);
            fprintf(stderr, "Error: Failed to close list\n");
            *did_error = true;
            return new_null();
        }
        if (delim->type == Comma) {
            continue;
        } else if (delim->type == Rbracket) {
            break;
        } else {
            free_json(&json);
            fprintf(stderr, "Error: unexpected token of type %s in list\n",
                    get_token_type_str(&delim->type));
            *did_error = true;
            return new_null();
        }
    }
    return json;
}

Json parse_json_from_tokens(Tokens *tokens, bool *did_error) {
    // printf("called parse_json_from_tokens\n");
    switch (peek_token(tokens)->type) {
    case Lcurly:
        return parse_json_object(tokens, did_error);
    case Lbracket:
        return parse_json_list(tokens, did_error);
    case Numeric_t:
        return parse_numeric_literal(next_token(tokens), did_error);
    case Keyword:
        return parse_keyword(next_token(tokens), did_error);
    case String_t:;
        const String *str = next_token(tokens)->data;
        return json_string_from(copy_heap_string(str));
    default:
        fprintf(stderr, "Error: Invalid expression first token: %s\n",
                get_token_type_str(&peek_token(tokens)->type));
        *did_error = true;
        return new_null();
    }
}

Json parse_json(const String *string, bool *did_error) {
    bool tokenization_error = false;
    Tokens tokens = tokenize_string(string, &tokenization_error);
    if (tokenization_error) {
        free_token_vec(&tokens.data);
        *did_error = true;
        return new_null();
    }
    // print_token_vec(&tokens.data);
    bool parsing_error = false;
    Json json = parse_json_from_tokens(&tokens, &parsing_error);
    // printf("back on parse_json\n");
    if (parsing_error) *did_error = true;
    free_token_vec(&tokens.data);
    // printf("returning from parse_json\n");
    return json;
}
