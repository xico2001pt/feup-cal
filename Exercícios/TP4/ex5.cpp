#include "exercises.h"
#include <vector>

unsigned long s_recursive(unsigned int n, unsigned int k) {
    if (k == 1 || k == n) return 1;
    return s_recursive(n - 1, k - 1) + k * s_recursive(n - 1, k);
}

unsigned long b_recursive(unsigned int n) {
    unsigned long result = 0;

    for (unsigned int k = 1; k <= n; ++k)
        result += s_recursive(n, k);
    return result;
}

unsigned long s_dynamic(unsigned int n, unsigned int k) {
    std::vector<unsigned long> memo(n - k + 1, 1);
    for (int i = 1; i < k; ++i) {
        for (int j = 1; j < memo.size(); ++j) {
            memo[j] = memo[j - 1] * (i + 1) + memo[j];
        }
    }
    return memo[n - k];
}

unsigned long b_dynamic(unsigned int n) {
    unsigned long result = 0;

    for (unsigned int k = 1; k <= n; ++k)
        result += s_dynamic(n, k);
    return result;
}

/// TESTS ///
#include <gtest/gtest.h>

TEST(TP4_Ex5, testPartitioningRecursive) {
    EXPECT_EQ(1,s_recursive(3,3));
    EXPECT_EQ(3025,s_recursive(9,3));
    EXPECT_EQ(22827,s_recursive(10,6));

    EXPECT_EQ(5,b_recursive(3));
    EXPECT_EQ(203,b_recursive(6));
    EXPECT_EQ(1382958545,b_recursive(15));
}

TEST(TP4_Ex5, testPartitioningDynamic) {
    EXPECT_EQ(1,s_dynamic(3,3));
    EXPECT_EQ(3025,s_dynamic(9,3));
    EXPECT_EQ(22827,s_dynamic(10,6));

    EXPECT_EQ(5,b_dynamic(3));
    EXPECT_EQ(203,b_dynamic(6));
    EXPECT_EQ(1382958545,b_dynamic(15));
}