#pragma once

#include "Scanner.hpp"
#include "Chunk.hpp"
#include "token.hpp"

typedef enum {
    PREC_NONE,
    PREC_ASGN,       // =                        (lowest)
    PREC_OR,         // ||
    PREC_AND,        // &&
    PREC_EQUALITY,   // == !=
    PREC_COMPARISON, // > < >= <=
    PREC_TERM,       // + -
    PREC_FACTOR,     // * / %
    PREC_UNARY,      // !
    PREC_CALL,       // foo()
    PREC_PRIMARY,    // literals, identifiers, ( ... )  (highest)
} Precedence;

class Compiler;

using ParseFn = void (Compiler::*)();

typedef struct {
   ParseFn prefix;
   ParseFn infix;
   Precedence precedence;
} ParseRule;

class Compiler {
    private:
        Scanner& scanner;
        Token curr, prev;
        bool hadError;
        /** 
            State that compiler enters when it encounters first error. 
            Means of performing syncronization.
        */
        bool panicMode;
        Chunk* compilingChunk;
        static ParseRule rules[static_cast<int>(TOKEN_EOF) + 1];

        static void ensureParseRules();
        ParseRule* getRule(TokenType type);

        void advance();
        void consume(TokenType type, const char* error);

        
        // erroring
        /** Errors at the current consumed token */
        void errorAtCurrent(const char* message);
        /** Errors at the previous consumed token */
        void error(const char* message);
        void errorAt(Token& token, const char* message);

        // parsers
        void expression();
        void parseAtPrecedence(Precedence precedence);
        void number();
        void grouping(); // parsers parenthesized expressions 
        void unary();
        void binary();

        // emitter 
        Chunk* currentChunk();
        void endCompiler();
        void emitReturn();
        void emitBytes(uint8_t byte1, uint8_t byte2);
        void emitByte(uint8_t);

        void emitConstant(double value);


    public:
        explicit Compiler(Scanner& scanner);
        bool compile(const char* source, Chunk& chunk);
};
