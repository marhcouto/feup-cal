#include "exercises.h"
#include <iostream>

bool isCanonical(unsigned int C[], unsigned int n) { //To check if a set of coins is a cannonical system
    unsigned int Stock[n];
    for (unsigned int i = 0; i < n; i++)
        Stock[i] = 100;
    for (unsigned int i = C[2] + 2; i < C[n-1] + C[n]; i++) {
        unsigned int arr1[n];
        unsigned int arr2[n];
        unsigned int bfCoins = 0, greedyCoins = 0;
        bool bf = changeMakingBF(C, Stock, n, i, arr1);
        bool greedy = changeMakingGreedy(C, Stock, n, i, arr2);
        if (bf && greedy) {
            for (unsigned int i = 0; i < n; i++) {
                greedyCoins += arr2[i];
                bfCoins += arr1[i];
            }
            if (bfCoins != greedyCoins) return false;
        }
        //else if (bf && !greedy) return false;
    }
    return true;
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
    EXPECT_EQ(isCanonical(C,3), false);
}