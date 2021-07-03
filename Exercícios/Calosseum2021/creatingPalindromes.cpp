#include <iostream>
#include <string>

int creatingPalindromes() {
    std::string original;
    int len;

    int dp[1001][1001];
    while (std::cin >> original) {
        len = original.size();

        for (int i = 0; i <= len; ++i) {
            for (int j = 0; j <= len; ++j) {
                if (i == 0) dp[i][j] = j;
                else if (j == 0) dp[i][j] = i;
                else if (original[i - 1] == original[len - j]) dp[i][j] = dp[i - 1][j - 1];
                else dp[i][j] = 1 + std::min(std::min(dp[i][j - 1], dp[i - 1][j]), dp[i - 1][j - 1]);
            }
        }
        std::cout << dp[len][len] / 2 << std::endl;
    }
    return 0;
}

/// TESTS ///
#include <gtest/gtest.h>

TEST(Calosseum2021, testCreatingPalindromes) {
    std::streambuf* orig = std::cin.rdbuf();
    std::istringstream input("bob\nmoo\napple\nbanana\nracecar\nabcdefghijklmnopqrstuvwxyzyxwvutsrqponmlkjihgfedca");
    std::cin.rdbuf(input.rdbuf());

    EXPECT_EQ(creatingPalindromes(), 0);

    std::cin.rdbuf(orig);
}