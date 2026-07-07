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
    this->stack.reset();
    return this->run();
}

InterpretResult VM::run() {
    uint8_t instr;

    for (;;) {
        #ifdef DEBUG_TRACE_EXECUTION
            if (!this->stack.empty()) {
                std::cout << "Stack: ";
                for (const Value* val = this->stack.begin(); val < this->stack.end(); val++) {
                    std::cout << "[ " << *val << " ]";
                }
                std::cout << "\n========================\n";
            }
            disassembleInstruction(*chunk, static_cast<size_t>(ip - chunk->code.data()));
        #endif

        instr = READ_BYTE();

        switch (instr) {
            case OP_RETURN:
                std::cout << this->stack.pop() << '\n';
                return INTERPRET_OK;
            case OP_CONST: {
                const uint8_t index = READ_BYTE();
                this->stack.push(chunk->constants[index]);
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
                this->stack.push(chunk->constants[index]);
                break;
            }
            case OP_NEGATE: {
                Value* val = this->stack.end() - 1;
                *val = -*val;
                break;
            }
            case OP_MUL: {
                BINARY_OP(*);
                break;
            }
            case OP_DIV: {
                BINARY_OP(/);
                break;
            }
            case OP_ADD: {
                BINARY_OP(+);
                break;
            }
            case OP_SUB: {
                BINARY_OP(-);
                break;
            }
        }
    }
}
