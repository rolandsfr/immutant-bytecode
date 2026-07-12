#pragma once

#include "Compiler.hpp"
#include "VM.hpp"

void runFile(const char* path, Compiler& compiler, VM& vm);