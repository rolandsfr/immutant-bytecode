#include <cstddef>
#include <vector>
#include <cstdint>

#include "Chunk.hpp"
#include "common.hpp"

/** Writes a byte to the chunk */
void Chunk::write(uint8_t byte, size_t line) {
    code.push_back(byte);
    write_line(line, code.size());
}

std::size_t Chunk::addConstant(Value value) {
    constants.push_back(value);
    return static_cast<size_t>(constants.size() - 1);
}

void Chunk::writeConstant(Value value, size_t line) {
    const int idx = addConstant(value);
    if(idx <= UINT8_MAX) {
        write(OP_CONST, line);
        write(idx, line);
    } else {
        write(OP_CONST_LONG, line);
        // split into 3 bytes
        write(static_cast<uint8_t>((idx >> 16) & 0xff), line);
        write(static_cast<uint8_t>((idx >> 8) & 0xff), line);
        write(static_cast<uint8_t>(idx & 0xff), line);
    }
}

uint8_t Chunk::readConstIdx8(size_t offset) {
    return get_code(offset + 1);
}

uint32_t Chunk::readConstIdx24(size_t offset) {
    uint32_t idx = 0;
    idx |= static_cast<uint32_t>(get_code(offset + 1)) << 16;
    idx |= static_cast<uint32_t>(get_code(offset + 2)) << 8;
    idx |= static_cast<uint32_t>(get_code(offset + 3));
    return idx;
}

Value Chunk::readConst8(size_t offset) {
    return constants.at(readConstIdx8(offset));
}

Value Chunk::readConst24(size_t offset, uint32_t* outIdx) {
    const uint32_t idx = readConstIdx24(offset);
    if (outIdx) {
        *outIdx = idx;
    }
    return constants.at(idx);
}

/** Returns number of bytes in the chunk */
std::size_t Chunk::count() {
    return static_cast<std::size_t>(code.size());
}

/** Returns code at offset (index) */
uint8_t Chunk::get_code(size_t index) {
    return code.at(index);
}

// ========== LINES =============

void Chunk::write_line(size_t line, size_t offset) {
    if(lines.empty() || lines.back().line != line) {
        lines.push_back({
            .offset = offset,
            .line = line
        });
    }
}

size_t Chunk::get_line(size_t offset) {
    if (lines.empty()) {
        return 0;
    }

    // write_line stores 1-based code.size(); offset is 0-based byte index
    const size_t target = offset + 1;

    int low = 0;
    int high = static_cast<int>(lines.size()) - 1;
    int result = 0;

    while (low <= high) {
        const int mid = low + (high - low) / 2;
        if (lines[mid].offset <= target) {
            result = mid;
            low = mid + 1;
        } else {
            high = mid - 1;
        }
    }

    return lines[result].line;
}
