#include <iomanip>
#include <iostream>
#include <string_view>

#include "Compiler.hpp"
#include "Scanner.hpp"
#include "token.hpp"

Compiler::Compiler(Scanner& scanner): scanner(scanner) {};

void Compiler::compile(const char* source) {
    scanner.init(source);

    // Temporary code
    int line = -1;
    Token token;

    for(;;) {
        token = scanner.scanToken();

        if(token.line != line) {
            std::cout << std::setw(4) << token.line << " ";
            line = token.line;
        } else {
            std::cout << "   | "; 
        }

        std::string_view lexeme(token.start, token.len);
        std::cout << std::setw(2) << static_cast<int>(token.type) << " '" << lexeme << "'\n";

        if(token.type == TOKEN_EOF) break;
    }
    
}