#include "exercises.h"

#include <vector>
#include <iostream>
#include <vector>
#include <fstream>

std::vector<int> computePrefix(std::string pattern) {
    std::vector<int> pi(pattern.length());
    pi[0] = -1;
    int k = -1;
    for (int q = 1; q < pattern.size(); ++q) {
        while (k > -1 && pattern[k+1] != pattern[q]) k = pi[k];
        if (pattern[k+1] == pattern[q]) k++;
        pi[q] = k;
    }
    return pi;
}

int kmpMatcher(std::string text, std::string pattern) {
    int res = 0;
    std::vector<int> pi = computePrefix(pattern);
    int q = -1;
    for (int i = 0; i < text.length(); ++i) {
        while (q > -1 && pattern[q+1] != text[i]) q = pi[q];
        if (pattern[q+1] == text[i]) q++;
        if (q == pattern.length()-1) {
            res++;
            q = pi[q];
        }
    }
    return res;
}

int numStringMatching(std::string filename, std::string toSearch) {
    std::ifstream file{filename};
    std::string str;
    std::vector<std::string> tokens;
    while(std::getline(file, str)){
        tokens.push_back(str);
    }

    int res = 0;
    for(const std::string &P : tokens) res += kmpMatcher(P, toSearch);
    return res;
}

/// TESTS ///
#include <gtest/gtest.h>

TEST(TP10_Ex1, testKmpMatcher) {
    EXPECT_EQ(3, kmpMatcher("aaabaabaacaabaa", "aabaa"));

    EXPECT_EQ(0, kmpMatcher("", "a"));
    EXPECT_EQ(1, kmpMatcher("a", "a"));
}

#define REL_PATH std::string("../TP10/") // relative path to the tests

TEST(TP10_Ex1, testNumStringMatching) {
    int num1 = numStringMatching(REL_PATH + "text1.txt", "estrutura de dados");
    EXPECT_EQ(3, num1);

    int num2=numStringMatching(REL_PATH +"text2.txt", "estrutura de dados");
    EXPECT_EQ(2,num2);
}