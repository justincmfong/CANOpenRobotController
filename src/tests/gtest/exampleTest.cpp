#include "gtest/gtest.h"

TEST(ExampleTest, First) {
    EXPECT_TRUE(true);
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}

// run using: clang++ -lgtest exampleTest.cpp -o  ./test -std=c++14 && ./test
// or g++