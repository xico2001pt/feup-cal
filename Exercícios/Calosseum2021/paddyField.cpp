#include <iostream>
#include <vector>
#include <algorithm>
#include <math.h>
#include <list>

#define INF 66666

typedef std::pair<int, int> Tuple;

static int nScarecrows;
static int fieldLength;
static int fieldWidth;

bool compare(const Tuple &t1, const Tuple &t2) {
    if (t1.first == t2.first) {
        return t1.second < t2.second;
    }
    return t1.first < t2.first;
}

int minimumCoverInterval(std::vector<bool> &booleans, std::list<Tuple> &intervals, int length) {
    for (bool boolean : booleans) {
        if (!boolean) return -1;
    }

    intervals.sort(compare);
    int start = 0, count = 0;
    auto it = intervals.begin();

    while(start < length){
        Tuple best; best.second = -INF;
        while(it != intervals.end() && it->first <= start){
            if(it->second > best.second){
                best = *it;
            }
            ++it;
        }
        count++;
        start = best.second + 1;
    }
    return count;
}

int paddyField() {
    while (!std::cin.eof()) {
        std::cin >> nScarecrows >> fieldLength >> fieldWidth;

        std::list<Tuple> intervals;
        std::vector<bool> booleans(fieldLength, false);
        double dist; int intDist; Tuple aux;
        for (int i = 0; i < nScarecrows; ++i) {
            std::cin >> aux.first >> aux.second; aux.first -= 1;
            dist = std::sqrt(std::max(0.0, std::pow(aux.second, 2) - std::pow(fieldWidth, 2) / 4.0));
            intDist = (int) dist;
            if (intDist == 0) continue;

            aux.second = std::min(aux.first + intDist, fieldLength - 1);
            aux.first = std::max(aux.first - intDist, 0);
            for (int j = aux.first; j <= aux.second; ++j) {
                booleans[j] = true;
            }

            intervals.push_back(aux);
        }

        int val = minimumCoverInterval(booleans, intervals, fieldLength);
        if (val == -1) std::cout << "X";
        else std::cout << val;
        std::cout << std::endl;
    }
    return 0;
}

/// TESTS ///
#include <gtest/gtest.h>

TEST(Calosseum2021, testPaddyField) {
    std::streambuf* orig = std::cin.rdbuf();
    std::istringstream input("3 6 2\n1 1\n3 1\n5 1\n2 6 2\n2 3\n5 2");
    std::cin.rdbuf(input.rdbuf());

    EXPECT_EQ(paddyField(), 0);

    std::cin.rdbuf(orig);
}