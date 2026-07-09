#include "VM.hpp"
#include "Compiler.hpp"

InterpretResult interpret(const char* source, Compiler& compiler) {
    compiler.compile(source);
    return INTERPRET_OK;
}
