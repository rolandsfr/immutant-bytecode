#include <cstring>

#include "Scanner.hpp"
#include "token.hpp"

void Scanner::init(const char* source) {
    this->start = source;
    this->current = source;
    this->line = 1;
}

Token Scanner::scanToken() {
    this->skipWhitespace();

    this->start = this->current;

    if (this->isAtEnd()) {
        return this->makeToken(TOKEN_EOF);
    }


    if(this->isNumber()) return this->number();

    char c = this->advance();

    switch (c) {
        case '(': return this->makeToken(TOKEN_LEFT_PAREN);
        case ')': return this->makeToken(TOKEN_RIGHT_PAREN);
        case '{': return this->makeToken(TOKEN_LEFT_BRACE);
        case '}': return this->makeToken(TOKEN_RIGHT_BRACE);
        case '+': return this->makeToken(TOKEN_PLUS);
        case '-': return this->makeToken(TOKEN_MINUS);
        case '*': return this->makeToken(TOKEN_STAR);
        case '%': return this->makeToken(TOKEN_MODULO);
        case ';': return this->makeToken(TOKEN_SEMICOLON);
        case '.': return this->makeToken(TOKEN_DOT);
        case ',': return this->makeToken(TOKEN_COMMA);

        case '!': return this->makeToken(this->match('=') ? TOKEN_BANG_EQUAL : TOKEN_BANG);
        case '=': return this->makeToken(this->match('=') ? TOKEN_EQUAL_EQUAL : TOKEN_EQUAL);
        case '<': return this->makeToken(this->match('=') ? TOKEN_LESS_EQUAL : TOKEN_LESS);
        case '>': return this->makeToken(this->match('=') ? TOKEN_GREATER_EQUAL : TOKEN_GREATER);
        case '/': return this->makeToken(TOKEN_SLASH);

        case '"': return this->string();


        default: return this->errorToken("Unexpected character.");
    }
}

bool Scanner::isAtEnd() {
    return *this->current == '\0';
}

char Scanner::advance() {
    return *this->current++;
}

bool Scanner::match(char expected) {
    if (this->isAtEnd()) return false;
    if (*this->current != expected) return false;
    this->current++;
    return true;
}

char Scanner::peek() {
    return *this->current;
}

char Scanner::peekNext() {
    if(isAtEnd()) return '\0';
    return *(this->current + 1);
}

void Scanner::skipWhitespace() {
    for (;;) {
        char c = this->peek();
        switch (c) {
            case ' ':
            case '\r':
            case '\t':
                this->advance();
                break;
            case '\n':
                this->line++;
                this->advance();
                break;
            case '/':
                if (this->peekNext() == '/') {
                    while (this->peek() != '\n' && !this->isAtEnd()) {
                        this->advance();
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
    while(this->peek() != '"' && !this->isAtEnd()) {
        if(this->peek() == '\n') this->line++;
        this->advance();
    }

    if(this->isAtEnd()) return errorToken("Unterminated string");
    this->advance();
    return makeToken(TOKEN_STRING);
}

Token Scanner::number() {
    if(this->peek() == '.') {
        this->advance();
        while(this->isDigit((this->peek()))) advance();
        return makeToken(TOKEN_NUMBER);
    } else {
        while (this->isDigit(this->peek())) this->advance(); 
    }

    if(this->peek() == '.' && isDigit(this->peekNext())) {
        this->advance();
        while(isDigit(this->peek())) advance();
    }

    return makeToken(TOKEN_NUMBER);
}

bool Scanner::isDigit(char c) {
    return c >= '0' && c <= '9';
}

bool Scanner::isNumber() {
    return this->isDigit(this->peek()) || (this->peek() == '.' && this->isDigit(this->peekNext()));
}


Token Scanner::makeToken(TokenType type) {
    Token token;
    token.type = type;
    token.line = this->line;
    token.start = this->start;
    token.len = static_cast<int>(this->current - this->start);
    return token;
}

Token Scanner::errorToken(const char* message) {
    Token token;
    token.type = TOKEN_ERR;
    token.line = this->line;
    token.start = message;
    token.len = static_cast<int>(std::strlen(message));
    return token;
}
