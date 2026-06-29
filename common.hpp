#pragma once

#include <cstddef>

using OpCode = enum {
    OP_RETURN,
    OP_CONST
};

using Line = struct {
    std::size_t offset;
    std::size_t line;
};