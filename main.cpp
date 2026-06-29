#include <iostream>

#include "Chunk.hpp"
#include "common.hpp"
#include "debug.hpp"

int main() {
    Chunk chunk;
    chunk.write(OP_RETURN, 2);

    std::size_t constant = chunk.addConstant(20);

    chunk.write(OP_CONST, 4);
    chunk.write(constant, 6);

    disassembleChunk(chunk, "test");
}