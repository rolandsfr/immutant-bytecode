#pragma once

typedef enum {
    // Single-character tokens.
    TOKEN_LEFT_PAREN,
    TOKEN_RIGHT_PAREN,
    TOKEN_LEFT_BRACE,
    TOKEN_RIGHT_BRACE,
    TOKEN_PLUS,
    TOKEN_MINUS,
    TOKEN_STAR,
    TOKEN_SLASH,
    TOKEN_MODULO,
    TOKEN_SEMICOLON,
    TOKEN_DOT,
    TOKEN_COMMA,

    // Comparison / logical tokens and assignment.
    TOKEN_BANG,
    TOKEN_BANG_EQUAL,
    TOKEN_EQUAL,
    TOKEN_EQUAL_EQUAL,
    TOKEN_GREATER,
    TOKEN_GREATER_EQUAL,
    TOKEN_LESS,
    TOKEN_LESS_EQUAL,
    TOKEN_AND,
    TOKEN_OR,

    // Literals
    TOKEN_IDENTIFIER,
    TOKEN_STRING,
    TOKEN_NUMBER,

    // Keywords
    TOKEN_FALSE,
    TOKEN_TRUE,
    TOKEN_NIL,
    TOKEN_WHILE,
    TOKEN_IF,
    TOKEN_ELSE,
    TOKEN_IMMUTANT,
    TOKEN_MUTANT,
    TOKEN_FN,
    TOKEN_RETURN,
    TOKEN_IMPURE,
    TOKEN_PURE,

    // End of file
    TOKEN_EOF,

    TOKEN_ERR
} TokenType;

typedef struct {
    TokenType type;
    const char* start;
    int len;
    int line;
} Token;
