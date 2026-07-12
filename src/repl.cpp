#include <iostream>
#include "interpret.hpp"

void repl(Compiler& compiler, VM& vm) {
    char buf[1024];
    for (;;) {
        std::cout << "> ";
        if (!std::cin.getline(buf, sizeof(buf))) {
            std::cout << '\n';
            break;
        }

        interpret(buf, compiler, vm);
    }
}
