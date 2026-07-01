#include <gtest/gtest.h>

#include "Chunk.hpp"
#include "common.hpp"

TEST(ChunkTest, EmptyChunkHasZeroBytes) {
    Chunk chunk;
    EXPECT_EQ(chunk.count(), 0u);
}

TEST(ChunkTest, WriteAppendsByteAndIncreasesCount) {
    Chunk chunk;
    chunk.write(42, 1);
    EXPECT_EQ(chunk.count(), 1u);
    EXPECT_EQ(chunk.get_code(0), 42);
}

TEST(ChunkTest, WriteMultipleBytesPreservesOrder) {
    Chunk chunk;
    chunk.write(OP_RETURN, 1);
    chunk.write(OP_CONST, 1);
    chunk.write(0, 1);

    EXPECT_EQ(chunk.count(), 3u);
    EXPECT_EQ(chunk.get_code(0), OP_RETURN);
    EXPECT_EQ(chunk.get_code(1), OP_CONST);
    EXPECT_EQ(chunk.get_code(2), 0);
}

TEST(ChunkTest, GetCodeOutOfRangeThrows) {
    Chunk chunk;
    EXPECT_THROW(chunk.get_code(0), std::out_of_range);
}

TEST(ChunkTest, WriteConstantAddsToPoolAndEmitsOpConst) {
    Chunk chunk;
    chunk.writeConstant(3.14, 1);

    ASSERT_EQ(chunk.constants.size(), 1u);
    EXPECT_DOUBLE_EQ(chunk.constants[0], 3.14);
    ASSERT_EQ(chunk.count(), 2u);
    EXPECT_EQ(chunk.get_code(0), OP_CONST);
    EXPECT_EQ(chunk.get_code(1), 0);
}

TEST(ChunkTest, WriteConstantAssignsSequentialIndices) {
    Chunk chunk;
    chunk.writeConstant(1.0, 1);
    chunk.writeConstant(2.0, 1);
    chunk.writeConstant(3.0, 1);

    ASSERT_EQ(chunk.constants.size(), 3u);
    EXPECT_DOUBLE_EQ(chunk.constants[0], 1.0);
    EXPECT_DOUBLE_EQ(chunk.constants[1], 2.0);
    EXPECT_DOUBLE_EQ(chunk.constants[2], 3.0);

    EXPECT_EQ(chunk.get_code(1), 0);
    EXPECT_EQ(chunk.get_code(3), 1);
    EXPECT_EQ(chunk.get_code(5), 2);
}

TEST(ChunkTest, WriteConstantUsesOpConstForIndex255) {
    Chunk chunk;
    for (size_t i = 0; i < 255; ++i) {
        chunk.writeConstant(static_cast<Value>(i), 1);
    }
    chunk.writeConstant(255.0, 1);

    ASSERT_EQ(chunk.constants.size(), 256u);
    const size_t lastInstructionOffset = chunk.count() - 2;
    EXPECT_EQ(chunk.get_code(lastInstructionOffset), OP_CONST);
    EXPECT_EQ(chunk.get_code(lastInstructionOffset + 1), 255);
}

TEST(ChunkTest, WriteConstantOverflowsToOpConstLongPastSimpleConst) {
    Chunk chunk;
    for (size_t i = 0; i < 255; ++i) {
        chunk.writeConstant(static_cast<Value>(i), 1);
    }
    chunk.writeConstant(255.0, 1);

    const size_t sizeAfter255 = chunk.count();
    EXPECT_EQ(chunk.get_code(sizeAfter255 - 2), OP_CONST);
    EXPECT_EQ(chunk.get_code(sizeAfter255 - 1), 255);

    chunk.writeConstant(256.0, 1);

    ASSERT_EQ(chunk.constants.size(), 257u);
    EXPECT_DOUBLE_EQ(chunk.constants[256], 256.0);
    EXPECT_EQ(chunk.count(), sizeAfter255 + 4u);
    EXPECT_EQ(chunk.get_code(sizeAfter255), OP_CONST_LONG);
    EXPECT_EQ(chunk.get_code(sizeAfter255 + 1), 0);
    EXPECT_EQ(chunk.get_code(sizeAfter255 + 2), 1);
    EXPECT_EQ(chunk.get_code(sizeAfter255 + 3), 0);
}

TEST(ChunkTest, WriteConstantUsesOpConstLongForIndex256) {
    Chunk chunk;
    for (size_t i = 0; i < 256; ++i) {
        chunk.writeConstant(static_cast<Value>(i), 1);
    }
    chunk.writeConstant(256.0, 1);

    ASSERT_EQ(chunk.constants.size(), 257u);
    const size_t lastInstructionOffset = chunk.count() - 4;
    EXPECT_EQ(chunk.get_code(lastInstructionOffset), OP_CONST_LONG);
    EXPECT_EQ(chunk.get_code(lastInstructionOffset + 1), 0);
    EXPECT_EQ(chunk.get_code(lastInstructionOffset + 2), 1);
    EXPECT_EQ(chunk.get_code(lastInstructionOffset + 3), 0);
}

TEST(ChunkTest, GetLineReturnsSourceLineForOffset) {
    Chunk chunk;
    chunk.write(OP_RETURN, 10);
    chunk.write(OP_CONST, 10);
    chunk.write(0, 10);

    EXPECT_EQ(chunk.get_line(0), 10u);
    EXPECT_EQ(chunk.get_line(1), 10u);
    EXPECT_EQ(chunk.get_line(2), 10u);
}

TEST(ChunkTest, GetLineUpdatesWhenSourceLineChanges) {
    Chunk chunk;
    chunk.write(OP_RETURN, 1);
    chunk.write(OP_RETURN, 2);

    EXPECT_EQ(chunk.get_line(0), 1u);
    EXPECT_EQ(chunk.get_line(1), 2u);
}

TEST(ChunkTest, GetLineCarriesForwardUntilNextLineMarker) {
    Chunk chunk;
    chunk.write(OP_RETURN, 5);
    chunk.write(OP_CONST, 5);
    chunk.write(0, 5);
    chunk.write(OP_RETURN, 9);

    EXPECT_EQ(chunk.get_line(0), 5u);
    EXPECT_EQ(chunk.get_line(1), 5u);
    EXPECT_EQ(chunk.get_line(2), 5u);
    EXPECT_EQ(chunk.get_line(3), 9u);
}
