#include "Compiler.hpp"
#include "Scanner.hpp"

Compiler::Compiler(Scanner& scanner): scanner(scanner) {};

void Compiler::compile(const char* source) {
    this->scanner.init(source);

    // TODO
}