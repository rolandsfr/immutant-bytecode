#include <cstdlib>

#include "Chunk.hpp"
#include "Compiler.hpp"
#include "Scanner.hpp"
#include "VM.hpp"
#include "repl.hpp"
#include "runFile.hpp"

void addSampleConstants(size_t amount, Chunk& chunk);

int main(int arg_c, char** argv) {
    Scanner scanner;
    Compiler compiler(scanner);
    VM vm;

    if(arg_c == 1) {
        repl(compiler);
    } else if(arg_c == 2) {
        runFile(argv[1], compiler);
    } else {
        std::exit(64);
    }
}
