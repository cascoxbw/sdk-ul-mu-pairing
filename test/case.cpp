#include <gtest/gtest.h>
#include "../lib/UlMuPair.hpp"

TEST(Pair, hello) 
{
    test();
    EXPECT_STRNE("hello", "world");
    EXPECT_EQ(7 * 6, 42);
}