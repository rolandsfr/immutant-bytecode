#pragma once

#include <cstdlib>
#include "token.hpp"

class Scanner {
    public:
        const char* start;
        const char* current;
        int line;

        void init(const char* source);

        Token scanToken();

    private:
        bool isAtEnd();
        char advance();
        bool match(char expected);
        char peek();
        char peekNext();

        void skipWhitespace();
        Token string();
        Token number();
        bool isNumber();
        bool isDigit(char c);

        Token makeToken(TokenType type);
        Token errorToken(const char* message);
};