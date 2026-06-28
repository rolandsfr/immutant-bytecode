#include <cstdint>
#include <iostream>
#include <iomanip>
#include "Chunk.hpp"
#include "common.hpp"

size_t disassembleInstruction(Chunk& chunk, size_t offset);
size_t simpleInstruction(const std::string& name, size_t offset);

void disassembleChunk(Chunk& chunk, const std::string& name) {
    std::cout << "== " << name << " ==\n";

    for (size_t offset = 0; offset < chunk.count();) {
        offset = disassembleInstruction(chunk, offset);
    }
}


size_t disassembleInstruction(Chunk& chunk, size_t offset) {
    std::cout << std::setfill('0') << std::setw(4) << offset << std::setfill(' ') << " ";

    uint8_t code = chunk.get_code(offset);
    switch (code) {
        case OP_RETURN: {
            return simpleInstruction("OP_RETURN", offset);
        }
        default: {
            std::cout << "Unknown instruction " << code;
            return offset + 1;
        }
    }
}

size_t simpleInstruction(const std::string& name, size_t offset) {
    std::cout << name << "\n";
    return offset + 1;
}