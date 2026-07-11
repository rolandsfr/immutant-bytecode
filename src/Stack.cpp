#include <cstdlib>
#include <iostream>

#include "Stack.hpp"

static std::size_t growCapacity(std::size_t capacity) {
    return capacity < 8 ? 8 : capacity * 2;
}

Stack::Stack() {
    capacity = STACK_INITIAL_CAPACITY;
    buffer = static_cast<Value*>(std::malloc(sizeof(Value) * capacity));
    top = buffer;
}

Stack::~Stack() {
    std::free(buffer);
}

void Stack::reset() {
    top = buffer;
}

void Stack::resize() {
    const std::size_t offset = static_cast<std::size_t>(top - buffer);
    capacity = growCapacity(capacity);
    Value* resized = static_cast<Value*>(
        std::realloc(buffer, sizeof(Value) * capacity));
    if (resized == nullptr) {
        std::cerr << "Stack allocation failed.\n";
        std::exit(EXIT_FAILURE);
    }
    buffer = resized;
    top = buffer + offset;
}

void Stack::push(Value value) {
    if (static_cast<std::size_t>(top - buffer) >= capacity) {
        resize();
    }

    *top = value;
    top++;
}

Value Stack::pop() {
    top--;
    return *top;
}

bool Stack::empty() const {
    return top == buffer;
}

std::size_t Stack::size() const {
    return static_cast<std::size_t>(top - buffer);
}

Value* Stack::begin() const {
    return buffer;
}

Value* Stack::end() const {
    return top;
}
