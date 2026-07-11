#include <cstring>
#include <string_view>

#include "Scanner.hpp"
#include "token.hpp"

void Scanner::init(const char* source) {
    start = source;
    current = source;
    line = 1;
}

Token Scanner::scanToken() {
    skipWhitespace();

    start = current;

    if (isAtEnd()) {
        return makeToken(TOKEN_EOF);
    }

    if (isNumber()) return number();

    char c = advance();

    if (isAlpha(c)) return identifier();

    switch (c) {
        case '(': return makeToken(TOKEN_LEFT_PAREN);
        case ')': return makeToken(TOKEN_RIGHT_PAREN);
        case '{': return makeToken(TOKEN_LEFT_BRACE);
        case '}': return makeToken(TOKEN_RIGHT_BRACE);
        case '+': return makeToken(TOKEN_PLUS);
        case '-': return makeToken(TOKEN_MINUS);
        case '*': return makeToken(TOKEN_STAR);
        case '%': return makeToken(TOKEN_MODULO);
        case ';': return makeToken(TOKEN_SEMICOLON);
        case '.': return makeToken(TOKEN_DOT);
        case ',': return makeToken(TOKEN_COMMA);

        case '!': return makeToken(match('=') ? TOKEN_BANG_EQUAL : TOKEN_BANG);
        case '=': return makeToken(match('=') ? TOKEN_EQUAL_EQUAL : TOKEN_EQUAL);
        case '<': return makeToken(match('=') ? TOKEN_LESS_EQUAL : TOKEN_LESS);
        case '>': return makeToken(match('=') ? TOKEN_GREATER_EQUAL : TOKEN_GREATER);
        case '/': return makeToken(TOKEN_SLASH);

        case '"': return string();

        default: return errorToken("Unexpected character.");
    }
}

bool Scanner::isAtEnd() {
    return *current == '\0';
}

char Scanner::advance() {
    return *current++;
}

bool Scanner::match(char expected) {
    if (isAtEnd()) return false;
    if (*current != expected) return false;
    current++;
    return true;
}

char Scanner::peek() {
    return *current;
}

char Scanner::peekNext() {
    if (isAtEnd()) return '\0';
    return *(current + 1);
}

void Scanner::skipWhitespace() {
    for (;;) {
        char c = peek();
        switch (c) {
            case ' ':
            case '\r':
            case '\t':
                advance();
                break;
            case '\n':
                line++;
                advance();
                break;
            case '/':
                if (peekNext() == '/') {
                    while (peek() != '\n' && !isAtEnd()) {
                        advance();
                    }
                } else {
                    return;
                }
                break;
            default:
                return;
        }
    }
}

Token Scanner::string() {
    while (peek() != '"' && !isAtEnd()) {
        if (peek() == '\n') line++;
        advance();
    }

    if (isAtEnd()) return errorToken("Unterminated string");
    advance();
    return makeToken(TOKEN_STRING);
}

Token Scanner::number() {
    if (peek() == '.') {
        advance();
        while (isDigit(peek())) advance();
        return makeToken(TOKEN_NUMBER);
    }

    while (isDigit(peek())) advance();

    if (peek() == '.' && isDigit(peekNext())) {
        advance();
        while (isDigit(peek())) advance();
    }

    return makeToken(TOKEN_NUMBER);
}

Token Scanner::identifier() {
    while (isAlpha(peek()) || isDigit(peek())) advance();
    return makeToken(identifierType());
}

TokenType Scanner::identifierType() {
    switch (start[0]) {
        // else
        case 'e': return checkKeyword(1, 3, "lse", TOKEN_ELSE);
        // mut
        case 'm': return checkKeyword(1, 2, "ut", TOKEN_MUTANT);
        // nil
        case 'n': return checkKeyword(1, 2, "il", TOKEN_NIL);
        // pure
        case 'p': return checkKeyword(1, 3, "ure", TOKEN_PURE);
        // return
        case 'r': return checkKeyword(1, 5, "eturn", TOKEN_RETURN);
        // true
        case 't': return checkKeyword(1, 3, "rue", TOKEN_TRUE);
        // while
        case 'w': return checkKeyword(1, 4, "hile", TOKEN_WHILE);
        case 'f': {
            if(current - start > 1) {
                switch (start[1]) {
                    // fn
                    case 'n': return checkKeyword(1, 1, "n", TOKEN_FN);
                    // false
                    case 'a': return checkKeyword(2, 3, "lse", TOKEN_FALSE);
                }
            }
        }
        case 'i': {
            if(current - start > 1) {
                switch (start[1]) {
                    // if
                    case 'f': return checkKeyword(1, 1, "f", TOKEN_IF);
                    case 'm': {
                        if(current - start > 2) {
                            switch (start[2]) {
                                // imut
                                case 'u': return checkKeyword(3, 1, "t", TOKEN_IMMUTANT);
                                // impure
                                case 'p': return checkKeyword(3, 3, "ure", TOKEN_IMPURE);
                            }
                        }
                    }
                }
            }
        }
    }
    return TOKEN_IDENTIFIER;
}

bool Scanner::isDigit(char c) {
    return c >= '0' && c <= '9';
}

bool Scanner::isNumber() {
    return isDigit(peek()) || (peek() == '.' && isDigit(peekNext()));
}

bool Scanner::isAlpha(char c) {
    return c >= 'a' && c <= 'z' || c >= 'A' && c <= 'Z' || c == '_';
}

TokenType Scanner::checkKeyword(int offset, int length, const char* rest, TokenType type) {
    const int lexemeLen = static_cast<int>(current - start);
    if (lexemeLen != offset + length) return TOKEN_IDENTIFIER;
    if (std::string_view(start + offset, length) != rest) return TOKEN_IDENTIFIER;
    return type;
}

Token Scanner::makeToken(TokenType type) {
    Token token;
    token.type = type;
    token.line = line;
    token.start = start;
    token.len = static_cast<int>(current - start);
    return token;
}

Token Scanner::errorToken(const char* message) {
    Token token;
    token.type = TOKEN_ERR;
    token.line = line;
    token.start = message;
    token.len = static_cast<int>(std::strlen(message));
    return token;
}
