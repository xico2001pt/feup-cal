#include "exercises.h"

unsigned int sumArray(unsigned int a[], unsigned int n) {
    unsigned int sum = 0;
    for(unsigned int i = 0; i < n; i++) {
        sum += a[i];
    }
    return sum;
}

#include <iostream>

bool isCanonical(unsigned int C[], unsigned int n) {
    bool canonical = true;
    unsigned int * Stock = (unsigned int *) malloc(sizeof(unsigned int) * n);
    unsigned int * usedCoins = (unsigned int *) malloc(sizeof(unsigned int) * n);
    unsigned int bruteCoins, greedyCoins;

    for (unsigned int i = 0; i < n; ++i)
        Stock[i] = 10;

    for (unsigned int i = (C[2] + 1) + 1; i < C[n - 2] + C[n - 1]; ++i) {
        if (!changeMakingBF(C, Stock, n, i, usedCoins)) {
            canonical = false;
            break;
        }
        bruteCoins = sumArray(usedCoins, n);

        if (!changeMakingGreedy(C, Stock, n, i, usedCoins)) {
            canonical = false;
            break;
        }
        greedyCoins = sumArray(usedCoins, n);

        if (bruteCoins < greedyCoins)
            break;
    }

    free(Stock);
    free(usedCoins);
    return canonical;
}


/// TESTS ///
#include <gtest/gtest.h>

TEST(TP1_Ex5, canonicalTrue) {
    unsigned int C[] = {1,2,5};
    EXPECT_EQ(isCanonical(C,3), true);

    unsigned int C2[] = {1,2,5,10};
    EXPECT_EQ(isCanonical(C2,4), true);
}

TEST(TP1_Ex5, canonicalFalse) {
    unsigned int C[] = {1,4,5};
    EXPECT_EQ(isCanonical(C,3), true);
}