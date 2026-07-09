#pragma once

#include "Scanner.hpp"

class Compiler {
    private:
        Scanner& scanner;
    public:
        explicit Compiler(Scanner& scanner);

        void compile(const char* source);
};