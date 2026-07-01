#include <gtest/gtest.h>
#include <sstream>
#include <string>
#include <vector>

#include "Chunk.hpp"
#include "common.hpp"
#include "debug.hpp"

size_t disassembleInstruction(Chunk& chunk, size_t offset);

namespace {

class CoutRedirect {
public:
    CoutRedirect() : old_(std::cout.rdbuf(buffer_.rdbuf())) {}
    ~CoutRedirect() { std::cout.rdbuf(old_); }

    std::string str() const { return buffer_.str(); }

private:
    std::stringstream buffer_;
    std::streambuf* old_;
};

void ExpectOutputContains(const std::string& output,
                          const std::vector<const char*>& substrings) {
    for (const char* part : substrings) {
        EXPECT_NE(output.find(part), std::string::npos)
            << "Expected output to contain: " << part << "\n"
            << "Actual output:\n"
            << output;
    }
}

}

TEST(DebugTest, DisassembleInstructionOpReturnAdvancesByOne) {
    Chunk chunk;
    chunk.write(OP_RETURN, 1);

    EXPECT_EQ(disassembleInstruction(chunk, 0), 1u);
}

TEST(DebugTest, DisassembleInstructionOpConstAdvancesByTwo) {
    Chunk chunk;
    chunk.write(OP_CONST, 1);
    chunk.write(0, 1);
    chunk.constants.push_back(1.0);

    EXPECT_EQ(disassembleInstruction(chunk, 0), 2u);
}

TEST(DebugTest, DisassembleInstructionOpConstLongAdvancesByFour) {
    Chunk chunk;
    for (size_t i = 0; i <= 256; ++i) {
        chunk.constants.push_back(static_cast<Value>(i));
    }
    chunk.write(OP_CONST_LONG, 1);
    chunk.write(0, 1);
    chunk.write(1, 1);
    chunk.write(0, 1);

    EXPECT_EQ(disassembleInstruction(chunk, 0), 4u);
}

TEST(DebugTest, DisassembleInstructionUnknownOpcodeAdvancesByOne) {
    Chunk chunk;
    chunk.write(99, 1);

    EXPECT_EQ(disassembleInstruction(chunk, 0), 1u);
}

TEST(DebugTest, DisassembleInstructionPrintsLineNumberOnFirstBytecodeOfLine) {
    Chunk chunk;
    chunk.write(OP_RETURN, 42);

    CoutRedirect capture;
    disassembleInstruction(chunk, 0);

    ExpectOutputContains(capture.str(), {"42", "OP_RETURN"});
}

TEST(DebugTest, DisassembleInstructionPrintsContinuationMarkerOnSameLine) {
    Chunk chunk;
    chunk.write(OP_RETURN, 1);
    chunk.write(OP_CONST, 1);
    chunk.write(0, 1);
    chunk.constants.push_back(3.14);

    CoutRedirect capture;
    disassembleInstruction(chunk, 1);

    ExpectOutputContains(capture.str(), {"|", "OP_CONST", "idx", "0", "3.14"});
}

TEST(DebugTest, DisassembleInstructionOpConstLongPrintsConstantInfo) {
    Chunk chunk;
    for (size_t i = 0; i < 256; ++i) {
        chunk.writeConstant(static_cast<Value>(i), 1);
    }
    chunk.writeConstant(256.0, 1);

    const size_t offset = chunk.count() - 4;

    CoutRedirect capture;
    disassembleInstruction(chunk, offset);

    ExpectOutputContains(capture.str(), {"|", "OP_CONST_LONG", "idx", "256"});
}

TEST(DebugTest, DisassembleInstructionUnknownOpcodePrintsMessage) {
    Chunk chunk;
    chunk.write(99, 5);

    CoutRedirect capture;
    disassembleInstruction(chunk, 0);

    ExpectOutputContains(capture.str(), {"Unknown instruction", "99"});
}

TEST(DebugTest, DisassembleChunkPrintsHeaderAndAllInstructions) {
    Chunk chunk;
    chunk.write(OP_RETURN, 10);
    chunk.write(OP_CONST, 10);
    chunk.write(0, 10);
    chunk.constants.push_back(1.5);

    CoutRedirect capture;
    disassembleChunk(chunk, "test");

    ExpectOutputContains(capture.str(), {"== test ==", "OP_RETURN", "OP_CONST", "1.5"});
}
