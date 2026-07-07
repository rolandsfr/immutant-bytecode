#pragma once

#include <cstddef>
#include <cstdint>

enum OpCode : uint8_t {
    OP_RETURN,
    OP_CONST,
    OP_CONST_LONG,
    OP_NEGATE,
    OP_ADD,
    OP_SUB,
    OP_MUL,
    OP_DIV
};

struct Line {
    std::size_t offset;
    std::size_t line;
};