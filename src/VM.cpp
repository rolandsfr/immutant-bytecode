#include <cstdint>
#include <iostream>

#include "VM.hpp"
#include "Chunk.hpp"
#include "common.hpp"

#ifdef DEBUG_TRACE_EXECUTION
#include "debug.hpp"
#endif

#define READ_BYTE() (*this->ip++)

VM::VM() {
    this->chunk = nullptr;
    this->ip = nullptr;
}

InterpretResult VM::interpret(Chunk& chunk) {
    this->chunk = &chunk;
    this->ip = chunk.code.data();
    return this->run();
}

InterpretResult VM::run() {
    uint8_t instr;

    for (;;) {
        #ifdef DEBUG_TRACE_EXECUTION
                disassembleInstruction(*chunk, static_cast<size_t>(ip - chunk->code.data()));
        #endif

        instr = READ_BYTE();

        switch (instr) {
            case OP_RETURN:
                return INTERPRET_OK;
            case OP_CONST: {
                const uint8_t index = READ_BYTE();
                std::cout << chunk->constants[index] << '\n';
                break;
            }
            case OP_CONST_LONG: {
                const uint8_t b0 = READ_BYTE();
                const uint8_t b1 = READ_BYTE();
                const uint8_t b2 = READ_BYTE();
                const uint32_t index =
                    (static_cast<uint32_t>(b0) << 16) |
                    (static_cast<uint32_t>(b1) << 8) |
                    b2;
                std::cout << chunk->constants[index] << '\n';
                break;
            }
        }
    }
}
