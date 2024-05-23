#ifndef PARSING_H_
#define PARSING_H_

#include <stdlib.h>
#include "string.h"
#include "json.h"
#include "vector.h"

typedef enum {
    Numeric_t,
    String_t,
    Keyword,
    Comma,
    Colon,
    Lbracket,
    Rbracket,
    Lcurly,
    Rcurly,
} TokenType;

typedef struct {
    TokenType type;
    String *data;
} Token;

typedef struct {
    Vec data;
    size_t curr;
} Tokens;

Json parse_json(const String *string, bool *did_error);

#endif // PARSING_H_
