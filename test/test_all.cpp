#include <gtest/gtest.h>
#include "../src/StatsHolderTmp.h"
#include "../src/StatsHolderResult.h"
// Demonstrate some basic assertions.
TEST(test, BasicAssertions) {
    // Expect two strings not to be equal.
    EXPECT_STRNE("hello", "world");
    // Expect equality.
    EXPECT_EQ(7 * 6, 42);
}