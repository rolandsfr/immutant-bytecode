#pragma once

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

        /** Add index of constant saved in the pool */
        std::size_t addConstant(Value value);

        /** Returns number of bytes in the chunk */
        std::size_t count() ;

        /** Returns code at index */
        uint8_t get_code(size_t index);

        size_t get_line(size_t offset);

    private:
        std::vector<Line> lines;

        void write_line(size_t line, size_t offset);

};