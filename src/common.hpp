#pragma once

#include <cstddef>

using OpCode = enum {
    OP_RETURN,
    OP_CONST,
    OP_CONST_LONG 
};

using Line = struct {
    std::size_t offset;
    std::size_t line;
};