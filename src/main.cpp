#include "Chunk.hpp"
#include "common.hpp"
#include "VM.hpp"

void addSampleConstants(size_t amount, Chunk& chunk);

int main() {
    Chunk chunk;
    chunk.writeConstant(32, 6);
    chunk.writeConstant(64, 7);
    chunk.write(OP_NEGATE, 11);
    chunk.write(OP_RETURN, 11);

    VM vm;
    vm.interpret(chunk);
}

// test function for long constants
void addSampleConstants(size_t amount, Chunk& chunk) {
    for(size_t i = 1; i < amount; i++) {
        chunk.writeConstant(i, i);
    }
}