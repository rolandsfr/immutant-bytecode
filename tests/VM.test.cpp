#include <gtest/gtest.h>
#include <sstream>
#include <string>

#include "Chunk.hpp"
#include "VM.hpp"
#include "common.hpp"

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

}  // namespace

TEST(VMTest, InterpretOpConstAndReturnReturnsOkAndPrintsValue) {
    Chunk chunk;
    chunk.writeConstant(32.0, 1);
    chunk.write(OP_RETURN, 2);

    CoutRedirect capture;
    VM vm;
    const InterpretResult result = vm.interpret(chunk);

    EXPECT_EQ(result, INTERPRET_OK);
    EXPECT_EQ(capture.str(), "32\n");
}

TEST(VMTest, InterpretOpConstLongAndReturnPrintsConstantAt256) {
    Chunk chunk;
    for (size_t i = 0; i <= 256; ++i) {
        chunk.constants.push_back(i == 256 ? 999.0 : static_cast<Value>(i));
    }
    chunk.write(OP_CONST_LONG, 1);
    chunk.write(0x00, 1);
    chunk.write(0x01, 1);
    chunk.write(0x00, 1);
    chunk.write(OP_RETURN, 2);

    CoutRedirect capture;
    VM vm;
    const InterpretResult result = vm.interpret(chunk);

    EXPECT_EQ(result, INTERPRET_OK);
    EXPECT_EQ(capture.str(), "999\n");
}

TEST(VMTest, InterpretOpNegateNegatesTopStackValue) {
    Chunk chunk;
    chunk.writeConstant(32.0, 1);
    chunk.writeConstant(64.0, 1);
    chunk.write(OP_NEGATE, 2);
    chunk.write(OP_RETURN, 3);

    CoutRedirect capture;
    VM vm;
    const InterpretResult result = vm.interpret(chunk);

    EXPECT_EQ(result, INTERPRET_OK);
    EXPECT_EQ(capture.str(), "-64\n");
}

TEST(VMTest, InterpretOpNegateOnNegativeValueProducesPositive) {
    Chunk chunk;
    chunk.writeConstant(-5.5, 1);
    chunk.write(OP_NEGATE, 2);
    chunk.write(OP_RETURN, 3);

    CoutRedirect capture;
    VM vm;
    const InterpretResult result = vm.interpret(chunk);

    EXPECT_EQ(result, INTERPRET_OK);
    EXPECT_EQ(capture.str(), "5.5\n");
}
