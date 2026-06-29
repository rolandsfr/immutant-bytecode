#include <cstdint>
#include <iostream>
#include <iomanip>
#include "Chunk.hpp"
#include "common.hpp"

size_t disassembleInstruction(Chunk& chunk, size_t offset);
size_t simpleInstruction(const std::string& name, size_t offset);
size_t constInstruction(const std::string& name, Chunk& chunk, size_t offset);

void disassembleChunk(Chunk& chunk, const std::string& name) {
    std::cout << "== " << name << " ==\n";

    for (size_t offset = 0; offset < chunk.count();) {
        offset = disassembleInstruction(chunk, offset);
    }
}


size_t disassembleInstruction(Chunk& chunk, size_t offset) {
    size_t line = chunk.get_line(offset);

    std::cout << std::setfill('0') << std::setw(4) << offset << std::setfill(' ') << " ";
    if(line != chunk.get_line(offset + 1)) {
        std::cout << std::right << std::setw(4) << line << " ";
    } else {
        std::cout << std::right << std::setw(5) << "| ";
    }

    uint8_t code = chunk.get_code(offset);

    switch (code) {
        case OP_RETURN: 
            return simpleInstruction("OP_RETURN", offset);
        case OP_CONST: 
            return constInstruction("OP_CONST", chunk, offset);
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

size_t constInstruction(const std::string& name, Chunk& chunk, size_t offset) {
    size_t constantCodeIdx = chunk.code.at(offset + 1);
    Value constant = chunk.constants.at(constantCodeIdx);

    std::cout << std::left << std::setw(16) << name << ' '
              << std::left << std::setw(4) << constant << '\n';

    return offset + 2;
}