#include <cstddef>
#include <vector>
#include <cstdint>
#include <iostream>

#include "Chunk.hpp"
#include "chunk.hpp"
#include "common.hpp"

/** Writes a byte to the chunk */
void Chunk::write(uint8_t byte, size_t line) {
    code.push_back(byte);
    this->write_line(line, code.size());
}

std::size_t Chunk::addConstant(Value value) {
    constants.push_back(value);
    return static_cast<size_t>(constants.size() - 1);
}

void Chunk::writeConstant(Value value, size_t line) {
    const int idx = this->addConstant(value);
    if(this->constants.size() <= UINT8_MAX) {
        this->code.push_back(OP_CONST);
        this->write(idx, line);
    } else {
        this->code.push_back(OP_CONST_LONG);
        // split into 3 bytes
        this->write(static_cast<uint8_t>((idx >> 16) & 0xff), line);
        this->write(static_cast<uint8_t>((idx >> 8) & 0xff), line);
        this->write(static_cast<uint8_t>(idx & 0xff), line);
    }
}

/** Returns number of bytes in the chunk */
std::size_t Chunk::count() {
    return static_cast<std::size_t>(code.size());
}

/** Returns code at offset (index) */
uint8_t Chunk::get_code(size_t index) {
    return this->code.at(index);
}

// ========== LINES =============

void Chunk::write_line(size_t line, size_t offset) {
    if(this->lines.empty() || this->lines.back().line != line) {
        this->lines.push_back({
            .offset = offset,
            .line = line
        });
    }
}

size_t Chunk::get_line(size_t offset) {
    int low = 0;
    int high = static_cast<int>(this->lines.size());

    while(low <= high) {
        int mid = (low + high) / 2;

        if(this->lines[mid].offset <= offset) {
            low = mid + 1;
        } else {
            high = mid - 1;
        }
    }

    return this->lines[low - 1].line;
}
