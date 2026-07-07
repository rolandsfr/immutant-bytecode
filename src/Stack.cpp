#include <cstdlib>
#include <iostream>

#include "Stack.hpp"

static std::size_t growCapacity(std::size_t capacity) {
    return capacity < 8 ? 8 : capacity * 2;
}

Stack::Stack() {
    this->capacity = STACK_INITIAL_CAPACITY;
    this->buffer = static_cast<Value*>(std::malloc(sizeof(Value) * this->capacity));
    this->top = this->buffer;
}

Stack::~Stack() {
    std::free(this->buffer);
}

void Stack::reset() {
    this->top = this->buffer;
}

void Stack::resize() {
    const std::size_t offset = static_cast<std::size_t>(this->top - this->buffer);
    this->capacity = growCapacity(this->capacity);
    Value* resized = static_cast<Value*>(
        std::realloc(this->buffer, sizeof(Value) * this->capacity));
    if (resized == nullptr) {
        std::cerr << "Stack allocation failed.\n";
        std::exit(EXIT_FAILURE);
    }
    this->buffer = resized;
    this->top = this->buffer + offset;
}

void Stack::push(Value value) {
    if (static_cast<std::size_t>(this->top - this->buffer) >= this->capacity) {
        this->resize();
    }

    *this->top = value;
    this->top++;
}

Value Stack::pop() {
    this->top--;
    return *this->top;
}

bool Stack::empty() const {
    return this->top == this->buffer;
}

std::size_t Stack::size() const {
    return static_cast<std::size_t>(this->top - this->buffer);
}

Value* Stack::begin() const {
    return this->buffer;
}

Value* Stack::end() const {
    return this->top;
}
