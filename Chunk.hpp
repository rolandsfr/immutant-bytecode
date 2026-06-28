#pragma once

#include <vector>
#include <cstdint>


/** 
    Chunk of linear assembly-like instructions
*/
class Chunk {
    public:
        std::vector<uint8_t> code;

        /** Writes a byte to the chunk */
        void write(uint8_t byte);

        /** Returns number of bytes in the chunk */
        std::size_t count() ;

        /** Returns code at index */
        uint8_t get_code(size_t index);
};