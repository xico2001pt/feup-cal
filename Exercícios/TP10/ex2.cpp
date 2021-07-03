#include "exercises.h"

#include <vector>
#include <iostream>
#include <fstream>
#include <algorithm>
#include <sstream>

int editDistance(std::string pattern, std::string text) {
    int n = pattern.size();
    int m = text.size();
    std::vector<std::vector<int> > D(n + 1, std::vector<int>(m + 1));
    for (int i = 0; i <= n; ++i) D[i][0] = i;
    for (int j = 0; j <= m; ++j) D[0][j] = j;

    pattern = " " + pattern;
    text = " " + text;
    for (int i = 1; i <= n; ++i) {
        for (int j = 1; j <= m; ++j) {
            if (pattern[i] == text[j]) {
                D[i][j] = D[i - 1][j - 1];
            }
            else {
                D[i][j] = 1 + std::min(std::min(D[i - 1][j - 1], D[i - 1][j]), D[i][j - 1]);
            }
        }
    }
    return D[n][m];
}

float numApproximateStringMatching(std::string filename, std::string toSearch) {
    std::ifstream file{filename};
    std::string str;
    std::vector<std::string> tokens;
    while(file >> str){
        tokens.push_back(str);
    }

    int res = 0;
    int counter = 0;
    for(const std::string &P : tokens) {
        res += editDistance(P, toSearch);
        counter++;
    }
    return (float) res / counter;
}

/// TESTS ///
#include <gtest/gtest.h>

TEST(TP10_Ex2, testEditDistance) {
    EXPECT_EQ(3, editDistance("abcdefghijkl", "bcdeffghixkl"));
    EXPECT_EQ(3, editDistance("bcdeffghixkl", "abcdefghijkl"));

    EXPECT_EQ(1, editDistance("", "b"));
    EXPECT_EQ(1, editDistance("a", ""));
    EXPECT_EQ(1, editDistance("a", "b"));
    EXPECT_EQ(0, editDistance("", ""));
    EXPECT_EQ(0, editDistance("a", "a"));
}

#define REL_PATH std::string("../TP10/") // relative path to the tests

TEST(TP10_Ex2, testNumApproximateStringMatching) {
    const float delta = 0.01;

    const float expected1 = 7.76;
    float dist1 = numApproximateStringMatching(REL_PATH + "text1.txt", "estrutur");
    ASSERT_LE(expected1-delta, dist1);
    ASSERT_GE(expected1+delta, dist1);

    const float expected2 = 7.49;
    float dist2 = numApproximateStringMatching(REL_PATH + "text2.txt", "estrutur");
    ASSERT_LE(expected2-delta, dist2);
    ASSERT_GE(expected2+delta, dist2);
}