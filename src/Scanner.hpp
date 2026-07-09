#pragma once

#include <cstdlib>

typedef enum {

} TokenType;

typedef struct {
    TokenType type;
    char* start;
    int len;
    int line;
} Token;

class Scanner {
    public:
        const char* start;
        const char* current;
        int line;

        void init(const char* source);
};