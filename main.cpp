#include <iostream>

#include "Chunk.hpp"
#include "common.hpp"
#include "debug.hpp"

int main() {
    Chunk chunk;
    chunk.write(OP_RETURN);

    std::cout << "Hello World!\n";

    disassembleChunk(chunk, "test");
}