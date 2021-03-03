#include "exercises.h"

int maxSubsequenceRec(int A[], unsigned int n, int &i, int &j) {
    if (j - i == 0) return A[i];

    int mid = i + (j - i) / 2;
    int lowBest, upBest, bestSum;

    int lowLeft = i, upLeft = mid;
    int leftSum = maxSubsequenceRec(A, n, lowLeft, upLeft);

    int lowRight = mid + 1, upRight = j;
    int rightSum = maxSubsequenceRec(A, n, lowRight, upRight);

    if (leftSum > rightSum) {
        bestSum = leftSum;
        lowBest = lowLeft;
        upBest = upLeft;
    }
    else {
        bestSum = rightSum;
        lowBest = lowRight;
        upBest = upRight;
    }

    int sum = 0, lowCenter = mid;
    int left = -99999;
    for (int a = mid; a >= i; --a) {
        sum += A[a];
        if (sum > left) {
            left = sum;
            lowCenter = a;
        }
    }

    sum = 0;
    int upCenter = mid + 1;
    int right = -99999;
    for (int a = mid + 1; a <= j; ++a) {
        sum += A[a];
        if (sum > right) {
            right = sum;
            upCenter = a;
        }
    }

    if (left + right > bestSum) {
        i = lowCenter;
        j = upCenter;
        return left + right;
    }
    else {
        i = lowBest;
        j = upBest;
        return bestSum;
    }
}

int maxSubsequenceDC(int A[], unsigned int n, int &i, int &j) {
    i = 0;
    j = n - 1;
    return maxSubsequenceRec(A, n, i ,j);
}

/// TESTS ///
#include <gtest/gtest.h>

TEST(TP3_Ex2, testMaxSubsequence) {
    int A1[] = {1, 2, 3, 4};
    unsigned int n1 = 4;
    int i, j;
    EXPECT_EQ(maxSubsequenceDC(A1,n1,i,j), 10);
    EXPECT_EQ(i, 0);
    EXPECT_EQ(j, 3);

    int A2[] = {2, -1, -3, 4};
    unsigned int n2 = 4;
    EXPECT_EQ(maxSubsequenceDC(A2,n2,i,j), 4);
    EXPECT_EQ(i, 3);
    EXPECT_EQ(j, 3);

    int A3[] = {2, -1, 3, 4};
    unsigned int n3 = 4;
    EXPECT_EQ(maxSubsequenceDC(A3,n3,i,j), 8);
    EXPECT_EQ(i, 0);
    EXPECT_EQ(j, 3);

    int A4[] = {-2, 1, -3, 4, -1, 2, 1, -5, 4};
    unsigned int n4 = 9;
    EXPECT_EQ(maxSubsequenceDC(A4,n4,i,j), 6);
    EXPECT_EQ(i, 3);
    EXPECT_EQ(j, 6);
}