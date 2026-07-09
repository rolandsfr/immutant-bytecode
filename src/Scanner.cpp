#include "Scanner.hpp"

void Scanner::init(const char* source) {
    this->start = source;
    this->current = source;
    this->line = 1;
}