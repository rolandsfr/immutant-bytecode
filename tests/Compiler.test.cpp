#include <gtest/gtest.h>
#include <sstream>
#include <string>
#include <vector>

#include "Chunk.hpp"
#include "Compiler.hpp"
#include "Scanner.hpp"
#include "VM.hpp"
#include "common.hpp"

namespace {

bool compileSource(const char* source, Chunk& chunk) {
    Scanner scanner;
    Compiler compiler(scanner);
    return compiler.compile(source, chunk);
}

void expectCodeBytes(Chunk& chunk, const std::vector<uint8_t>& bytes) {
    ASSERT_EQ(chunk.count(), bytes.size());
    for (size_t i = 0; i < bytes.size(); ++i) {
        EXPECT_EQ(chunk.get_code(i), bytes[i]);
    }
}

class CoutRedirect {
public:
    CoutRedirect() : old_(std::cout.rdbuf(buffer_.rdbuf())) {}
    ~CoutRedirect() { std::cout.rdbuf(old_); }

    std::string str() const { return buffer_.str(); }

private:
    std::stringstream buffer_;
    std::streambuf* old_;
};

}  // namespace

TEST(CompilerTest, CompileEmptySourceReturnsFalse) {
    Chunk chunk;
    EXPECT_FALSE(compileSource("", chunk));
}

TEST(CompilerTest, CompileWhitespaceOnlyReturnsFalse) {
    Chunk chunk;
    EXPECT_FALSE(compileSource("   \t\n", chunk));
}

TEST(CompilerTest, CompileNumberLiteralEmitsConstAndReturn) {
    Chunk chunk;
    ASSERT_TRUE(compileSource("42", chunk));

    ASSERT_EQ(chunk.constants.size(), 1u);
    EXPECT_DOUBLE_EQ(chunk.constants[0], 42.0);
    expectCodeBytes(chunk, {OP_CONST, 0, OP_RETURN});
}

TEST(CompilerTest, CompileDecimalNumberLiteral) {
    Chunk chunk;
    ASSERT_TRUE(compileSource("3.14", chunk));

    ASSERT_EQ(chunk.constants.size(), 1u);
    EXPECT_DOUBLE_EQ(chunk.constants[0], 3.14);
    expectCodeBytes(chunk, {OP_CONST, 0, OP_RETURN});
}

TEST(CompilerTest, CompileAdditionEmitsAddOpcode) {
    Chunk chunk;
    ASSERT_TRUE(compileSource("1 + 2", chunk));

    ASSERT_EQ(chunk.constants.size(), 2u);
    EXPECT_DOUBLE_EQ(chunk.constants[0], 1.0);
    EXPECT_DOUBLE_EQ(chunk.constants[1], 2.0);
    expectCodeBytes(chunk, {OP_CONST, 0, OP_CONST, 1, OP_ADD, OP_RETURN});
}

TEST(CompilerTest, CompileSubtractionEmitsSubOpcode) {
    Chunk chunk;
    ASSERT_TRUE(compileSource("5 - 3", chunk));

    expectCodeBytes(chunk, {OP_CONST, 0, OP_CONST, 1, OP_SUB, OP_RETURN});
}

TEST(CompilerTest, CompileMultiplicationEmitsMulOpcode) {
    Chunk chunk;
    ASSERT_TRUE(compileSource("4 * 5", chunk));

    expectCodeBytes(chunk, {OP_CONST, 0, OP_CONST, 1, OP_MUL, OP_RETURN});
}

TEST(CompilerTest, CompileDivisionEmitsDivOpcode) {
    Chunk chunk;
    ASSERT_TRUE(compileSource("8 / 2", chunk));

    expectCodeBytes(chunk, {OP_CONST, 0, OP_CONST, 1, OP_DIV, OP_RETURN});
}

TEST(CompilerTest, CompileRespectsMulOverAddPrecedence) {
    Chunk chunk;
    ASSERT_TRUE(compileSource("1 + 2 * 3", chunk));

    ASSERT_EQ(chunk.constants.size(), 3u);
    EXPECT_DOUBLE_EQ(chunk.constants[0], 1.0);
    EXPECT_DOUBLE_EQ(chunk.constants[1], 2.0);
    EXPECT_DOUBLE_EQ(chunk.constants[2], 3.0);
    expectCodeBytes(chunk,
                    {OP_CONST, 0, OP_CONST, 1, OP_CONST, 2, OP_MUL, OP_ADD, OP_RETURN});
}

TEST(CompilerTest, CompileUnaryMinusEmitsNegate) {
    Chunk chunk;
    ASSERT_TRUE(compileSource("-7", chunk));

    ASSERT_EQ(chunk.constants.size(), 1u);
    EXPECT_DOUBLE_EQ(chunk.constants[0], 7.0);
    expectCodeBytes(chunk, {OP_CONST, 0, OP_NEGATE, OP_RETURN});
}

TEST(CompilerTest, CompileGroupingParsesParentheses) {
    Chunk chunk;
    ASSERT_TRUE(compileSource("(1 + 2)", chunk));

    expectCodeBytes(chunk, {OP_CONST, 0, OP_CONST, 1, OP_ADD, OP_RETURN});
}

TEST(CompilerTest, CompileUnaryMinusOnGroupedExpression) {
    Chunk chunk;
    ASSERT_TRUE(compileSource("-(1 + 2)", chunk));

    expectCodeBytes(chunk, {OP_CONST, 0, OP_CONST, 1, OP_ADD, OP_NEGATE, OP_RETURN});
}

TEST(CompilerTest, CompileTrailingOperatorReturnsFalse) {
    Chunk chunk;
    EXPECT_FALSE(compileSource("1 +", chunk));
}

TEST(CompilerTest, CompileExtraTokensAfterExpressionReturnsFalse) {
    Chunk chunk;
    EXPECT_FALSE(compileSource("1 2", chunk));
}

TEST(CompilerTest, CompileUnclosedParenReturnsFalse) {
    Chunk chunk;
    EXPECT_FALSE(compileSource("(1 + 2", chunk));
}

TEST(CompilerTest, CompiledExpressionProducesCorrectVmResult) {
    Chunk chunk;
    ASSERT_TRUE(compileSource("1 + 2 * 3", chunk));

    CoutRedirect capture;
    VM vm;
    EXPECT_EQ(vm.interpret(chunk), INTERPRET_OK);
    EXPECT_EQ(capture.str(), "7\n");
}

TEST(CompilerTest, CompiledUnaryMinusRunsInVm) {
    Chunk chunk;
    ASSERT_TRUE(compileSource("-(10 - 3)", chunk));

    CoutRedirect capture;
    VM vm;
    EXPECT_EQ(vm.interpret(chunk), INTERPRET_OK);
    EXPECT_EQ(capture.str(), "-7\n");
}
