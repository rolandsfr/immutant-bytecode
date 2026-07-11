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
        // navigation
        bool isAtEnd();
        char advance();
        bool match(char expected);
        char peek();
        char peekNext();

        // utils
        bool isNumber();
        bool isDigit(char c);
        bool isAlpha(char c);
        /** Skips `\n`, `\t`, `\r` and comments */
        void skipWhitespace();
        /** 
            Returns `keyword` if rest symbols starting with `start` match.
            Otherwise returns `TOKEN_IDENTIFIER`
          */
        TokenType checkKeyword(int offset, int length, const char* rest, TokenType type);
        TokenType identifierType();

        // token emitters
        Token string();
        Token number();
        Token identifier();

        // factories
        Token makeToken(TokenType type);
        Token errorToken(const char* message);
};