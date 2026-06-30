#include "Chunk.hpp"
#include "common.hpp"
#include "debug.hpp"
#include <cstddef>

void addSampleConstants(size_t amount, Chunk& chunk);

int main() {
    Chunk chunk;
    chunk.write(OP_RETURN, 2);
    chunk.writeConstant(32, 6);
    chunk.writeConstant(64, 6);

    disassembleChunk(chunk, "test");
}

// test function for long constants
void addSampleConstants(size_t amount, Chunk& chunk) {
    for(size_t i = 1; i < amount; i++) {
        chunk.writeConstant(i, i);
    }
}