#include "exercises.h"

int maxSubsequence(int A[], unsigned int n, unsigned int &i, unsigned int &j) {
    int maxSum = 0;
    int currentSum;

    for (unsigned int a = 0; a < n; ++a) {
        currentSum = 0;
        for (unsigned int b = a; b < n; ++b) {
            currentSum += A[b];
            if (currentSum > maxSum) {
                maxSum = currentSum;
                i = a;
                j = b;
            }
        }
    }
    return maxSum;
}


/// TESTS ///
#include <gtest/gtest.h>

TEST(TP1_Ex2, maxSubsequence) {
    int A[] = {-2, 1, -3, 4, -1, 2, 1, -5, 4};
    unsigned int n = 9;
    unsigned int i, j;
    EXPECT_EQ(maxSubsequence(A,n,i,j), 6);
    EXPECT_EQ(i, 3);
    EXPECT_EQ(j, 6);
}