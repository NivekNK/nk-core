#include <gtest/gtest.h>

#include "test.h"

TEST(MyTest, GetMyTest) {
    void* testing = test_nullptr();
    EXPECT_EQ(testing, nullptr);
}
