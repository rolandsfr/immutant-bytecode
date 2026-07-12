#include "interpret.hpp"

InterpretResult interpret(const char* source, Compiler& compiler, VM& vm) {
    Chunk chunk;
    if (!compiler.compile(source, chunk)) return INTERPRET_COMPILE_ERR;
    return vm.interpret(chunk);
}
