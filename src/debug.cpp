#include <cstdint>
#include <iostream>
#include <iomanip>

#include "Chunk.hpp"
#include "common.hpp"

size_t disassembleInstruction(Chunk& chunk, size_t offset);
size_t simpleInstruction(const std::string& name, size_t offset);
size_t constInstruction(const std::string& name, Chunk& chunk, size_t offset);
size_t constLongInstruction(const std::string& name, Chunk& chunk, size_t offset);

void disassembleChunk(Chunk& chunk, const std::string& name) {
    std::cout << "== " << name << " ==\n";

    for (size_t offset = 0; offset < chunk.count();) {
        offset = disassembleInstruction(chunk, offset);
    }
}


size_t disassembleInstruction(Chunk& chunk, size_t offset) {
    size_t line = chunk.get_line(offset);

    std::cout << std::right << std::setfill('0') << std::setw(4) << offset << std::setfill(' ') << " ";
    if (offset == 0 || line != chunk.get_line(offset - 1)) {
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
        case OP_CONST_LONG: 
            return constLongInstruction("OP_CONST_LONG", chunk, offset);
        case OP_NEGATE:
            return simpleInstruction("OP_NEGATE", offset);
        case OP_ADD:
            return simpleInstruction("OP_ADD", offset);
        case OP_SUB:
            return simpleInstruction("OP_SUB", offset);
        case OP_MUL:
            return simpleInstruction("OP_MUL", offset);
        case OP_DIV:
            return simpleInstruction("OP_DIV", offset);
        default: {
            std::cout << "Unknown instruction " << static_cast<int>(code) << '\n';
            return offset + 1;
        }
    }
}

size_t simpleInstruction(const std::string& name, size_t offset) {
    std::cout << name << "\n";
    return offset + 1;
}

size_t constInstruction(const std::string& name, Chunk& chunk, size_t offset) {
    const size_t constantCodeIdx = chunk.readConstIdx8(offset);
    const Value constant = chunk.readConst8(offset);

    std::cout << std::left << std::setw(16) << name << ' '
              << std::left << std::setw(2) << "idx " <<  constantCodeIdx << std::setw(0) << " -> " << constant << '\n' << std::resetiosflags(std::ios_base::adjustfield);

    return offset + 2;
}

size_t constLongInstruction(const std::string& name, Chunk& chunk, size_t offset) {
    uint32_t idx;
    const Value constant = chunk.readConst24(offset, &idx);

     std::cout << std::left << std::setw(16) << name << ' '
              << std::left << std::setw(2) << "idx " <<  idx << std::setw(0) << " -> " << constant << '\n' << std::resetiosflags(std::ios_base::adjustfield);

    // operator byte + 3 separate value bytes
    return offset + 4;
}