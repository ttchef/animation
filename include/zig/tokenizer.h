#ifndef TOKENIZER_H
#define TOKENIZER_H

#include <stdint.h>
#include <stdlib.h>

typedef struct Tokenizer {} Tokenizer;

typedef uint8_t Token;

enum {
    TOKEN_EOF = 0, // End of file
    TOKEN_INVALID = 1,
    TOKEN_IDENTIFIER = 2,
    TOKEN_NUMBER_LITERAL = 3,
    TOKEN_STRING_LITERAL = 4,
    TOKEN_CHAR_LITERAL = 5,

    TOKEN_DOT = '.',
    TOKEN_COMMA = ',',
    TOKEN_COLON = ':',
    TOKEN_BANG = '!',
    TOKEN_PIPE = '|',
    TOKEN_EQUAL = '=',
    TOKEN_SEMICOLON = ';',
    TOKEN_AMPERSAND = '&',
    TOKEN_QUESTION_MARK = '?',
    TOKEN_PERCENT = '%',
    TOKEN_PLUS = '+',
    TOKEN_MINUS = '-',
    TOKEN_ASTERISK = '*',
    TOKEN_SLASH = '/',
    TOKEN_L_PAREN = '(',
    TOKEN_R_PAREN = ')',
    TOKEN_L_BRACE = '{',
    TOKEN_R_BRACE = '}',
    TOKEN_L_BRACKET = '[',
    TOKEN_R_BRACKET = ']',
    TOKEN_L_ANGLE_BRACKET = '<',
    TOKEN_R_ANGLE_BRACKET = '>',
};

void tokenizer_init(Tokenizer* tokenizer, char* slice);

Token tokenizer_next(Tokenizer* tokenizer);

char* tokenizer_current(Tokenizer* tokenizer);

size_t tokenizer_start(Tokenizer* tokenizer);
size_t tokenizer_end(Tokenizer* tokenizer);

#endif // TOKENIZER_H
