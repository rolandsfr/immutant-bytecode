#pragma once

#include "Chunk.hpp"

enum InterpretResult {
    INTERPRET_OK,
    INTERPRET_COMPILE_ERR,
    INTERPRET_RUNTIME_ERR,
};

class VM {
    public:
        InterpretResult interpret(Chunk& chunk);
        VM();

    private:
        uint8_t* ip;
        Chunk* chunk;

        InterpretResult run();
};