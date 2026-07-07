#pragma once

#include <string>
#include "Chunk.hpp"

void disassembleChunk(Chunk& chunk, const std::string& name);

size_t disassembleInstruction(Chunk& chunk, size_t offset);