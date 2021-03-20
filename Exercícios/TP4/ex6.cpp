#include "exercises.h"
#include <iomanip>
#include <chrono>
#include <fstream>
#include <iostream>

int maxSubsequenceDP(int A[], unsigned int n, unsigned int &i, unsigned int &j) {
    int soma = 0, bestSoma = 0;
    unsigned int auxI = 0;
    for (unsigned int idx = 0; idx < n; idx++) {
        soma += A[idx];
        if (soma < 0) {
            soma = 0;
            auxI = idx + 1;
        }
        else if (soma > bestSoma) {
            bestSoma = soma;
            i = auxI;
            j = idx;
        }
    }
    return bestSoma;
}

int maxSubsequenceBF(int A[], unsigned int n, unsigned int &i, unsigned int &j) {
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

int maxSubsequenceRec(int A[], unsigned int n, int &i, int &j) {
    if (j - i == 0) return A[i];

    int mid = i + (j - i) / 2;
    int lowBest, upBest;
    int bestSum;

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
    return maxSubsequenceRec(A, n, i , j);
}

void testPerformanceMaxSubsequence() {
    srand(time(NULL));
    int sequence[10000];
    unsigned int c, d;
    int a, b;

    std::ofstream file("testPerformance6.csv");
    file << std::fixed;
    file << std::setprecision(3);
    file << "n,miliBF,miliDC,miliDP\n";

    for (unsigned int n = 500; n <= 10000; n += 500) {
        int averageBF = 0, averageDC = 0, averageDP = 0;
        for (int k = 0; k < 100; ++k) {
            for (int i = 0; i < n; ++i) {
                sequence[i] = (int) (rand() % (2 * n)) - (int) n;
            }

            auto startBF = std::chrono::high_resolution_clock::now();
            maxSubsequenceBF(sequence, n, c, d);
            auto finishBF = std::chrono::high_resolution_clock::now();
            auto miliBF = std::chrono::duration_cast<std::chrono::milliseconds>(finishBF - startBF).count();
            averageBF += (int) miliBF;

            auto startDC = std::chrono::high_resolution_clock::now();
            maxSubsequenceDC(sequence, n, a, b);
            auto finishDC = std::chrono::high_resolution_clock::now();
            auto miliDC = std::chrono::duration_cast<std::chrono::milliseconds>(finishDC - startDC).count();
            averageDC += (int) miliDC;

            auto startDP = std::chrono::high_resolution_clock::now();
            maxSubsequenceDP(sequence, n, c, d);
            auto finishDP = std::chrono::high_resolution_clock::now();
            auto miliDP = std::chrono::duration_cast<std::chrono::milliseconds>(finishDP - startDP).count();
            averageDP += (int) miliDP;
        }
        file << n << "," << averageBF / 100 << "," << averageDC / 100 << "," << averageDP / 100 << std::endl;
        std::cout << "Done n=" << n << std::endl;
    }
    file.close();
}

/// TESTS ///
#include <gtest/gtest.h>

TEST(TP4_Ex6, testMaxSubsequence) {
    int A1[] = {1, 2, 3, 4};
    unsigned int n1 = 4;
    unsigned int i, j;
    EXPECT_EQ(maxSubsequenceDP(A1,n1,i,j), 10);
    EXPECT_EQ(i, 0);
    EXPECT_EQ(j, 3);

    int A2[] = {2, -1, -3, 4};
    unsigned int n2 = 4;
    EXPECT_EQ(maxSubsequenceDP(A2,n2,i,j), 4);
    EXPECT_EQ(i, 3);
    EXPECT_EQ(j, 3);

    int A3[] = {2, -1, 3, 4};
    unsigned int n3 = 4;
    EXPECT_EQ(maxSubsequenceDP(A3,n3,i,j), 8);
    EXPECT_EQ(i, 0);
    EXPECT_EQ(j, 3);

    int A4[] = {-2, 1, -3, 4, -1, 2, 1, -5, 4};
    unsigned int n4 = 9;
    EXPECT_EQ(maxSubsequenceDP(A4,n4,i,j), 6);
    EXPECT_EQ(i, 3);
    EXPECT_EQ(j, 6);

    //testPerformanceMaxSubsequence();
}