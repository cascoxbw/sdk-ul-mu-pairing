#include <gtest/gtest.h>
#include "../lib/UlMuPair.hpp"

TEST(Pair, main) 
{
    sInput in;
    sOutput out;
    UlMuPair(&in,&out);
    EXPECT_EQ(0, 0);
}