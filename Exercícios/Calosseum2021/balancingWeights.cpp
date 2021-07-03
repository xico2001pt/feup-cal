#include <iostream>
#include <string>
#include <sstream>

static int mass;
static int n;
static int weights[12];
static bool used[12];
static std::string last;

bool balancingWeightsRec(int i, int sumSel, int sumLeft) {
    if (sumSel == mass) {
        std::stringstream res;
        int k;
        for (k = 0; k < i; ++k) {
            if (used[k]) {res << weights[k]; break;}
        }
        for (++k; k < i; ++k) {
            if (used[k]) res << " + " << weights[k];
        }
        std::string s(res.str());
        if (s != last)
            std::cout << s << std::endl;
        last = s;
        return true;
    }
    if (i == n) return false;

    if (sumSel + sumLeft < mass) return false;

    bool found = false;
    int newLeft = sumLeft, lastWeight = 0;

    for (int cur = i; cur < n; ++cur) {
        if (lastWeight != weights[cur]) {
            used[cur] = true;
            lastWeight = weights[cur];
            newLeft -= lastWeight;
            if (balancingWeightsRec(cur + 1, sumSel + lastWeight, newLeft)) found = true;
        }
        used[cur] = false;
    }

    return found;
}

int balancingWeights() {
    while (true) {
        std::cin >> mass;
        if (mass == 0) break;
        std::cin >> n;
        int w, totalSum = 0;
        for (int i = 0; i < n; ++i) {
            std::cin >> w;
            weights[i] = w;
            totalSum += w;
        }
        last = "";
        std::cout << "Weights for " << mass << ":\n";
        if(!balancingWeightsRec(0, 0, totalSum))
            std::cout << "No solutions\n";
    }
    return 0;
}

/// TESTS ///
#include <gtest/gtest.h>

TEST(Calosseum2021, testBalancingWeights) {
    std::streambuf* orig = std::cin.rdbuf();
    std::istringstream input("4 8 4 3 2 2 1 1 1 1\n5 1 6\n600 12 100 100 100 100 90 90 80 70 70 60 50 50\n0");
    std::cin.rdbuf(input.rdbuf());

    EXPECT_EQ(balancingWeights(), 0);

    std::cin.rdbuf(orig);
}