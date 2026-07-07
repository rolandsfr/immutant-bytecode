#pragma once

#include <cstddef>

#include "Chunk.hpp"

#define STACK_INITIAL_CAPACITY 256

class Stack {
    public:
        Stack();
        ~Stack();

        void reset();
        void push(Value value);
        Value pop();

        bool empty() const;
        std::size_t size() const;
        Value* begin() const;
        Value* end() const;

    private:
        void resize();

        Value* buffer;
        Value* top;
        std::size_t capacity;
};
