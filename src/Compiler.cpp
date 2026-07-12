#include <cstdlib>
#include <string_view>
#include <iostream>
#include <string>

#include "Compiler.hpp"
#include "Scanner.hpp"
#include "common.hpp"
#include "token.hpp"

#ifdef DEBUG_PRINT_CODE
#include "debug.hpp"
#endif

namespace {
constexpr int RULE_COUNT = static_cast<int>(TOKEN_EOF) + 1;
constexpr ParseRule NO_PARSE_RULE = {nullptr, nullptr, PREC_NONE};
}  // namespace

ParseRule Compiler::rules[RULE_COUNT];

Compiler::Compiler(Scanner& scanner): scanner(scanner) {};


ParseRule* Compiler::getRule(TokenType type) {
    ensureParseRules();
    return &rules[type];
}

bool Compiler::compile(const char* source, Chunk& chunk) {
    scanner.init(source);

    hadError = false;
    panicMode = false;
    compilingChunk = &chunk;

    advance();
    expression();
    consume(TOKEN_EOF, "Expected end of file");
    endCompiler();
    return !hadError;
}

void Compiler::advance() {
    prev = curr;

    for(;;) {
        curr = scanner.scanToken();
        if(curr.type != TOKEN_ERR) break;
        errorAtCurrent(curr.start);
    }
}

void Compiler::consume(TokenType type, const char* error) {
    if(curr.type == type) {
        advance();
        return;
    }

    errorAtCurrent(error);
}

void Compiler::errorAtCurrent(const char* message) {
    errorAt(curr, message);
}

void Compiler::error(const char* message) {
    errorAt(prev, message);
}

void Compiler::errorAt(Token& token, const char* message) {
    if(panicMode) return;

    panicMode = true;
    std::cerr << "[line " << token.line << "] Error";
    if(token.type == TOKEN_EOF) {
        std::cerr << " at end";
    } else if(token.type == TOKEN_ERR) {} // no-op
    else {
        const auto lexeme = std::string_view(token.start, token.len);
        std::cerr << " at '" << lexeme << "'";
    }

    std::cerr << ": " << message << "\n";
    hadError = true;
}

//  ======= parser ========

void Compiler::number() {
    double value = std::stod(prev.start, NULL);
    this->emitConstant(value);
}

void Compiler::grouping() {
    expression();
    consume(TOKEN_RIGHT_PAREN, "Expected ')' after expression.");
}

void Compiler::unary() {
    TokenType type = prev.type;

    // compile operand
    parseAtPrecedence(PREC_UNARY);
    
    switch (type) {
        case TOKEN_MINUS: {
            emitByte(OP_NEGATE);
            break;
        }
        default:
            break;
    }
}

void Compiler::binary() {
   // memorize op token
   TokenType type = prev.type;
   
   // parse operand
   ParseRule* rule = getRule(type);
   if(rule) {
        parseAtPrecedence(static_cast<Precedence>(rule->precedence + 1));
   } else {
        errorAt(prev, "Could not find rule for token");
   }

   switch(type) {
        case TOKEN_PLUS: {
            this->emitByte(OP_ADD);
            break;
        }
        case TOKEN_MINUS: {
            this->emitByte(OP_SUB);
            break;
        }
        case TOKEN_STAR: {
            this->emitByte(OP_MUL);
            break;
        }
        case TOKEN_SLASH: {
            this->emitByte(OP_DIV);
            break;
        }
        default:
            return;
   }
}

void Compiler::parseAtPrecedence(Precedence precedence) {
    advance();

    ParseFn prefixRule = getRule(prev.type)->prefix;
    if (!prefixRule) {
        error("Expected expression.");
        return;
    }

    (this->*prefixRule)();

    while(precedence <= getRule(curr.type)->precedence) {
        advance();
        ParseFn infixRule = getRule(prev.type)->infix;
        (this->*infixRule)();
    }
  
}

void Compiler::expression() {
    parseAtPrecedence(PREC_ASGN);
}

//  ======= backend ========

void Compiler::emitConstant(double value) {
    compilingChunk->writeConstant(value, prev.line);
}

Chunk* Compiler::currentChunk() {
    return compilingChunk;
}


void Compiler::endCompiler() {
    this->emitReturn();
    #ifdef DEBUG_PRINT_CODE
    if(!hadError) {
        disassembleChunk(*compilingChunk, "code");
    }
    #endif
}

void Compiler::emitReturn() {
    this->emitByte(OP_RETURN);
}

void Compiler::emitBytes(uint8_t byte1, uint8_t byte2) {
    emitByte(byte1);
    emitByte(byte2);
}

void Compiler::emitByte(uint8_t byte) {
    compilingChunk->write(byte, prev.line);
}

void Compiler::ensureParseRules() {
    static bool initialized = false;
    if (initialized) return;
    initialized = true;

    rules[TOKEN_LEFT_PAREN] = {&Compiler::grouping, nullptr, PREC_NONE};
    rules[TOKEN_RIGHT_PAREN] = NO_PARSE_RULE;
    rules[TOKEN_LEFT_BRACE] = NO_PARSE_RULE;
    rules[TOKEN_RIGHT_BRACE] = NO_PARSE_RULE;
    rules[TOKEN_PLUS] = {nullptr, &Compiler::binary, PREC_TERM};
    rules[TOKEN_MINUS] = {&Compiler::unary, &Compiler::binary, PREC_TERM};
    rules[TOKEN_STAR] = {nullptr, &Compiler::binary, PREC_FACTOR};
    rules[TOKEN_SLASH] = {nullptr, &Compiler::binary, PREC_FACTOR};
    rules[TOKEN_MODULO] = {nullptr, &Compiler::binary, PREC_FACTOR};
    rules[TOKEN_SEMICOLON] = NO_PARSE_RULE;
    rules[TOKEN_DOT] = NO_PARSE_RULE;
    rules[TOKEN_COMMA] = NO_PARSE_RULE;
    rules[TOKEN_BANG] = {&Compiler::unary, nullptr, PREC_NONE};
    rules[TOKEN_BANG_EQUAL] = {nullptr, &Compiler::binary, PREC_EQUALITY};
    rules[TOKEN_EQUAL] = {nullptr, &Compiler::binary, PREC_ASGN};
    rules[TOKEN_EQUAL_EQUAL] = {nullptr, &Compiler::binary, PREC_EQUALITY};
    rules[TOKEN_GREATER] = {nullptr, &Compiler::binary, PREC_COMPARISON};
    rules[TOKEN_GREATER_EQUAL] = {nullptr, &Compiler::binary, PREC_COMPARISON};
    rules[TOKEN_LESS] = {nullptr, &Compiler::binary, PREC_COMPARISON};
    rules[TOKEN_LESS_EQUAL] = {nullptr, &Compiler::binary, PREC_COMPARISON};
    rules[TOKEN_AND] = {nullptr, &Compiler::binary, PREC_AND};
    rules[TOKEN_OR] = {nullptr, &Compiler::binary, PREC_OR};
    rules[TOKEN_IDENTIFIER] = NO_PARSE_RULE;
    rules[TOKEN_STRING] = NO_PARSE_RULE;
    rules[TOKEN_NUMBER] = {&Compiler::number, nullptr, PREC_NONE};
    rules[TOKEN_FALSE] = NO_PARSE_RULE;
    rules[TOKEN_TRUE] = NO_PARSE_RULE;
    rules[TOKEN_NIL] = NO_PARSE_RULE;
    rules[TOKEN_WHILE] = NO_PARSE_RULE;
    rules[TOKEN_IF] = NO_PARSE_RULE;
    rules[TOKEN_ELSE] = NO_PARSE_RULE;
    rules[TOKEN_IMMUTANT] = NO_PARSE_RULE;
    rules[TOKEN_MUTANT] = NO_PARSE_RULE;
    rules[TOKEN_FN] = NO_PARSE_RULE;
    rules[TOKEN_RETURN] = NO_PARSE_RULE;
    rules[TOKEN_IMPURE] = NO_PARSE_RULE;
    rules[TOKEN_PURE] = NO_PARSE_RULE;
    rules[TOKEN_EOF] = NO_PARSE_RULE;
}