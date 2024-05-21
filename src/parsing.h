#ifndef PARSING_H_
#define PARSING_H_

#include <stdlib.h>
#include "string.h"
#include "json.h"
#include "vector.h"

typedef enum {
    Int_t,
    Float_t,
    String_t,
    Null_t,
    Comma,
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

Json parse_json(const String* string);

#endif // PARSING_H_
