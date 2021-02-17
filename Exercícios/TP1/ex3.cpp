#include "exercises.h"

static bool found;

void copyArr(unsigned int from[], unsigned int to[], unsigned int n) {
    for (unsigned int i = 0; i < n; ++i) {
        to[i] = from[i];
    }
}

unsigned int calcSum(unsigned int num[], unsigned int n) {
    unsigned int result = 0;
    for (unsigned int i = 0; i < n; ++i) {
        result += num[i];
    }
    return result;
}

void changeRec(unsigned int C[], unsigned int Stock[], unsigned int n, unsigned int T, unsigned int usedCoins[], unsigned int current, unsigned int total, unsigned int best[]) {
    if (current == n) {
        if (total == T) {
            if (!found) {
                found = true;
                copyArr(usedCoins, best, n);
            }
            else {
                if (calcSum(usedCoins, n) < calcSum(best, n)) {
                    copyArr(usedCoins, best, n);
                }
            }
        }
        return;
    }

    for (unsigned int i = 0; i <= Stock[current]; ++i) {
        usedCoins[current] = i;
        changeRec(C, Stock, n, T, usedCoins, current + 1, total + i * C[current], best);
    }
}

bool changeMakingBF(unsigned int C[], unsigned int Stock[], unsigned int n, unsigned int T, unsigned int usedCoins[]) {
    unsigned int best[n];
    found = false;
    changeRec(C, Stock, n, T, usedCoins, 0, 0, best);
    if (found) {
        copyArr(best, usedCoins, n);
    }
    return found;
}

/// TESTS ///
#include <gtest/gtest.h>

TEST(TP1_Ex3, hasBFChangeCanonical) {
    unsigned int C[] = {1,2,5,10};
    unsigned int Stock[] = {1,1,1,1};
    unsigned int n = 4;
    unsigned int usedCoins[4];

    EXPECT_EQ(changeMakingBF(C,Stock,n,13,usedCoins), true);
    EXPECT_EQ(usedCoins[0], 1);
    EXPECT_EQ(usedCoins[1], 1);
    EXPECT_EQ(usedCoins[2], 0);
    EXPECT_EQ(usedCoins[3], 1);

    unsigned int Stock2[] = {1,2,4,2};
    EXPECT_EQ(changeMakingBF(C,Stock2,n,38,usedCoins), true);
    EXPECT_EQ(usedCoins[0], 1);
    EXPECT_EQ(usedCoins[1], 1);
    EXPECT_EQ(usedCoins[2], 3);
    EXPECT_EQ(usedCoins[3], 2);
}

TEST(TP1_Ex3, hasBFChangeNonCanonical) {
    unsigned int C[] = {1,4,5};
    unsigned int Stock[] = {2,2,1};
    unsigned int n = 3;
    unsigned int usedCoins[3];

    EXPECT_EQ(changeMakingBF(C,Stock,n,6,usedCoins), true);
    EXPECT_EQ(usedCoins[0], 1);
    EXPECT_EQ(usedCoins[1], 0);
    EXPECT_EQ(usedCoins[2], 1);

    EXPECT_EQ(changeMakingBF(C,Stock,n,8,usedCoins), true);
    EXPECT_EQ(usedCoins[0], 0);
    EXPECT_EQ(usedCoins[1], 2);
    EXPECT_EQ(usedCoins[2], 0);
}

TEST(TP1_Ex3, hasNoBFChange) {
    unsigned int C[] = {1,2,5,10};
    unsigned int Stock[] = {0,1,1,1};
    unsigned int n = 4;
    unsigned int usedCoins[4];

    EXPECT_EQ(changeMakingBF(C,Stock,n,18,usedCoins), false);
    EXPECT_EQ(changeMakingBF(C,Stock,n,1,usedCoins), false);
}