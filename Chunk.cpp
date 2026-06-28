#include <cstddef>
#include <vector>
#include <cstdint>

#include "Chunk.hpp"
#include "chunk.hpp"

/** Writes a byte to the chunk */
void Chunk::write(uint8_t byte) {
    code.push_back(byte);
}

/** Returns number of bytes in the chunk */
std::size_t Chunk::count() {
    return static_cast<std::size_t>(code.size());
}

uint8_t Chunk::get_code(size_t index) {
    return code.at(index);
}
