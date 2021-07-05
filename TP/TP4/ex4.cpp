#include "exercises.h"
#include <sstream>

std::string calcSum(int sequence[], unsigned long n) {

    unsigned int indexes[n];
    unsigned int sums[n];
    unsigned int bestSums[n];

    //1-Initialize
    for (unsigned int i = 0; i < n; i++) {
        indexes[i] = 0;
        sums[i] = 0;
        bestSums[i] = 999999;
    }

    //2-Calculate best sums
    for (unsigned int m = 1; m <= n; m++) {
        for (unsigned int i = 0; i <= n - m; i++) {
            sums[i] += sequence[i + m - 1];
            if (sums[i] < bestSums[m - 1]) {
                bestSums[m - 1] = sums[i];
                indexes[m - 1] = i;
            }
        }
    }

    //3-Turn sums into string
    std::stringstream answer;
    for (unsigned int i = 0; i < n; i++)
        answer << bestSums[i] << "," << indexes[i] << ";";

    return answer.str();
}

void testPerformanceCalcSum() {
    //TODO
}

/// TESTS ///
#include <gtest/gtest.h>

TEST(TP4_Ex4, CalcSumArrayTest) {
    int sequence[5] = {4,7,2,8,1};
    int sequence2[9] = {6,1,10,3,2,6,7,2,4};

    EXPECT_EQ("1,4;9,1;11,2;18,1;22,0;",calcSum(sequence, 5));
    EXPECT_EQ("1,1;5,3;11,3;16,1;20,3;24,3;31,1;35,1;41,0;",calcSum(sequence2, 9));
	
	testPerformanceCalcSum();
}