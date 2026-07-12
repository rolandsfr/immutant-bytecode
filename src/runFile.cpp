#include <fstream>
#include <ios>
#include <iostream>
#include <string.h>
#include <cstdlib>

#include "VM.hpp"
#include "interpret.hpp"
#include "Compiler.hpp"

std::string static readFile(const char* path);

void runFile(const char* path, Compiler& compiler, VM& vm) {
    std::string source = readFile(path);
    InterpretResult result = interpret(&source[0], compiler, vm);

    if(result == INTERPRET_COMPILE_ERR) std::exit(65);
    if(result == INTERPRET_RUNTIME_ERR) std::exit(70);
}

std::string static readFile(const char* path) {
    std::ifstream file(path, std::ios::binary);
    
    if(!file) {
        std::cerr << "Could not open the file " << path << '\n';
        exit(74);
    }

    file.seekg(0, std::ios::end);
    size_t size = file.tellg();
    const auto pos = file.tellg();
    if (pos < 0) {
        std::cerr << "Could not read the file " << path << '\n';
        std::exit(74);
    }

    std::string buf(size, ' ');
    file.seekg(0);
    file.read(&buf[0], size);

    if(file.gcount() != static_cast<std::streamsize>(size)) {
        std::cerr << "Could not read the file " << path << '\n';
        exit(74); 
    }

    return buf;
}