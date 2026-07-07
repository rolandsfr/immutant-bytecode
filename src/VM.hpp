#pragma once

#include "Chunk.hpp"
#include "Stack.hpp"

enum InterpretResult {
    INTERPRET_OK,
    INTERPRET_COMPILE_ERR,
    INTERPRET_RUNTIME_ERR,
};

#define BINARY_OP(op) \
    do { \
        double b = this->stack.pop(); \
        double a = this->stack.pop(); \
        this->stack.push(a op b);    \
    } while (0); \

class VM {
    public:
        InterpretResult interpret(Chunk& chunk);
        VM();

    private:
        uint8_t* ip;
        Chunk* chunk;
        Stack stack;

        InterpretResult run();
};
