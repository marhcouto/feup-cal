#include "exercises.h"
#include <iostream>

bool changeMakingBacktrackingRec(unsigned int C[], unsigned int Stock[], unsigned int n, unsigned int T,
                                 unsigned int currentIndex, unsigned int curNCoins,
                                 unsigned int curCoins[], unsigned int &minCoins, unsigned int bestCoins[]) {

    if (currentIndex == n) {
        if (T == 0) {
            if (curNCoins < minCoins) {
                minCoins = curNCoins;
                for (unsigned int i = 0; i < n; i++)
                    bestCoins[i] = curCoins[i];
            }
            return true;
        }
        return false;
    }
    //Try including a coin with a value C[currentIndex]
    bool foundSolutionWithCoin = false;

    // Include coin
    if (curCoins[currentIndex] < Stock[currentIndex] && T >= C[currentIndex]) {
        curCoins[currentIndex]++;
        foundSolutionWithCoin = changeMakingBacktrackingRec(C, Stock, n, T - C[currentIndex], currentIndex, curNCoins + 1, curCoins, minCoins, bestCoins);
        curCoins[currentIndex]--;
    }

    // Do not include
    bool foundSolutionWithoutCoin = changeMakingBacktrackingRec(C, Stock, n, T, currentIndex + 1, curNCoins, curCoins, minCoins, bestCoins);

    return foundSolutionWithCoin || foundSolutionWithoutCoin;
}

bool changeMakingBacktracking(unsigned int C[], unsigned int Stock[], unsigned int n, unsigned int T, unsigned int usedCoins[]) {
    unsigned int minCoins = 0;
    unsigned int candidate[20];
    for (unsigned int i = 0; i < n; i++) {
        candidate[i] = 0;
        minCoins += Stock[i];
    }
    return changeMakingBacktrackingRec(C, Stock, n, T, 0, 0, candidate, minCoins, usedCoins);
}


/// TESTS ///
#include <gtest/gtest.h>

TEST(TP2_Ex3, hasBFChangeCanonical) {
    unsigned int C[] = {1,2,5,10};
    unsigned int Stock[] = {1,1,1,1};
    unsigned int n = 4;
    unsigned int usedCoins[4];

    EXPECT_EQ(changeMakingBacktracking(C,Stock,n,13,usedCoins), true);
    EXPECT_EQ(usedCoins[0], 1);
    EXPECT_EQ(usedCoins[1], 1);
    EXPECT_EQ(usedCoins[2], 0);
    EXPECT_EQ(usedCoins[3], 1);

    unsigned int Stock2[] = {1,2,4,2};
    EXPECT_EQ(changeMakingBacktracking(C,Stock2,n,38,usedCoins), true);
    EXPECT_EQ(usedCoins[0], 1);
    EXPECT_EQ(usedCoins[1], 1);
    EXPECT_EQ(usedCoins[2], 3);
    EXPECT_EQ(usedCoins[3], 2);

    unsigned int Stock3[] = {10,2,4,2};
    EXPECT_EQ(changeMakingBacktracking(C,Stock3,n,10,usedCoins), true);
    EXPECT_EQ(usedCoins[0], 0);
    EXPECT_EQ(usedCoins[1], 0);
    EXPECT_EQ(usedCoins[2], 0);
    EXPECT_EQ(usedCoins[3], 1);
}

TEST(TP2_Ex3, hasBFChangeNonCanonical) {
    unsigned int C[] = {1,4,5};
    unsigned int Stock[] = {2,2,1};
    unsigned int n = 3;
    unsigned int usedCoins[3];

    EXPECT_EQ(changeMakingBacktracking(C,Stock,n,6,usedCoins), true);
    EXPECT_EQ(usedCoins[0], 1);
    EXPECT_EQ(usedCoins[1], 0);
    EXPECT_EQ(usedCoins[2], 1);

    EXPECT_EQ(changeMakingBacktracking(C,Stock,n,8,usedCoins), true);
    EXPECT_EQ(usedCoins[0], 0);
    EXPECT_EQ(usedCoins[1], 2);
    EXPECT_EQ(usedCoins[2], 0);

    unsigned int Stock2[] = {8,2,1};
    EXPECT_EQ(changeMakingBacktracking(C,Stock2,n,7,usedCoins), true);
    EXPECT_EQ(usedCoins[0], 2);
    EXPECT_EQ(usedCoins[1], 0);
    EXPECT_EQ(usedCoins[2], 1);

    EXPECT_EQ(changeMakingBacktracking(C,Stock2,n,8,usedCoins), true);
    EXPECT_EQ(usedCoins[0], 0);
    EXPECT_EQ(usedCoins[1], 2);
    EXPECT_EQ(usedCoins[2], 0);
}

TEST(TP2_Ex3, hasNoBFChange) {
    unsigned int C[] = {1,2,5,10};
    unsigned int Stock[] = {0,1,1,1};
    unsigned int n = 4;
    unsigned int usedCoins[4];

    EXPECT_EQ(changeMakingBacktracking(C,Stock,n,18,usedCoins), false);
    EXPECT_EQ(changeMakingBacktracking(C,Stock,n,1,usedCoins), false);
}