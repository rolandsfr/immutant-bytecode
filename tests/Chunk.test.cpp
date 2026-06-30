#include <gtest/gtest.h>

// Demonstrate some basic assertions.
TEST(ChunkTest, BasicAssertions) {
  // Expect two strings not to be equal.
  EXPECT_STRNE("hellos", "world");
  // Expect equality.
  EXPECT_EQ(7 * 6, 412);
}