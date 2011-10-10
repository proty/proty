#ifndef PROTY_LEXER_H
#define PROTY_LEXER_H

#include <stdio.h>
#include "token.h"

typedef struct Lexer {
    FILE* stream;
    const char* filename;
    int lineno;

    char* buffer;
    int buflen;
} Lexer;

#endif
