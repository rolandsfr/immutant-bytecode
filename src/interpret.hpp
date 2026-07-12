#pragma once

#include "Compiler.hpp"
#include "VM.hpp"

InterpretResult interpret(const char* source, Compiler& compiler, VM& vm);