#include "exercises.h"
#include <vector>
#include <iostream>
#include <fstream>
#include <chrono>
#include <cstdlib>
#include <ctime>
#include <iomanip>

std::string calcSum(int sequence[], unsigned long n) {
    std::vector<unsigned long> s(n, 0);
    std::string result = "";
    unsigned long bestSum, bestI, sum;

    for (unsigned long m = 0; m < n; ++m) {
        bestSum = 9999999; bestI = -1;
        for (unsigned long i = 0; i < n - m; ++i) {
            sum = s[i] + sequence[i + m];
            if (sum < bestSum) {
                bestSum = sum;
                bestI = i;
            }
            s[i] = sum;
        }
        result += std::to_string(bestSum) + "," + std::to_string(bestI) + ";";
    }
    return result;
}

void testPerformanceCalcSum() {
    srand(time(NULL));
    int sequence[10000];

    std::ofstream file("testPerformance4.csv");
    file << std::fixed;
    file << std::setprecision(3);
    file << "n,mili\n";

    for (unsigned long n = 500; n <= 10000; n += 500) {
        int average = 0;
        for (int k = 0; k < 100; ++k) {
            for (int i = 0; i < n; ++i) {
                sequence[i] = (int) (rand() % (10 * n)) + 1;
            }

            auto start = std::chrono::high_resolution_clock::now();
            calcSum(sequence, n);
            auto finish = std::chrono::high_resolution_clock::now();

            auto mili = std::chrono::duration_cast<std::chrono::milliseconds>(finish - start).count();
            average += (int) mili;
        }
        file << n << "," << average / 100 << std::endl;
    }
    file.close();
}

/// TESTS ///
#include <gtest/gtest.h>

TEST(TP4_Ex4, CalcSumArrayTest) {
    int sequence[5] = {4,7,2,8,1};
    int sequence2[9] = {6,1,10,3,2,6,7,2,4};

    EXPECT_EQ("1,4;9,1;11,2;18,1;22,0;",calcSum(sequence, 5));
    EXPECT_EQ("1,1;5,3;11,3;16,1;20,3;24,3;31,1;35,1;41,0;",calcSum(sequence2, 9));
	
	//testPerformanceCalcSum();
}