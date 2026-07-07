#pragma once

#include <cstddef>
#include <vector>
#include <cstdint>
#include "common.hpp"

using Value = double;

/** 
    Chunk of linear assembly-like instructions
*/
class Chunk {
    public:
        std::vector<uint8_t> code;
        std::vector<Value> constants;

        /** Writes a byte to the chunk */
        void write(uint8_t byte, size_t line);

        /* 
            Writes constant.
            Decides appropriate byte length based on current count.
        */
        void writeConstant(Value value, size_t line);

        // Decode constant operands at opcode byte offset in code.
        uint8_t readConstIdx8(size_t offset);
        uint32_t readConstIdx24(size_t offset);
        Value readConst8(size_t offset);
        Value readConst24(size_t offset, uint32_t* outIdx);

        /** Returns number of bytes in the chunk */
        std::size_t count();

        /** Returns code at index */
        uint8_t get_code(size_t index);

        size_t get_line(size_t offset);

    private:
        std::vector<Line> lines;

        /** Save constant to the pool */
        std::size_t addConstant(Value value);

        void write_line(size_t line, size_t offset);

};
