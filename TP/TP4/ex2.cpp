#include "exercises.h"

bool changeMakingUnlimitedDP(unsigned int C[], unsigned int n, unsigned int T, unsigned int usedCoins[]) {
    const unsigned int INF_N_COINS = T + 1;
    unsigned int minCoins[1001]; //assumes T <= 1000
    unsigned int lastCoin[1001];

    //Step 1: Initialize the DP table
	minCoins[0] = 0;
	for (unsigned int k = 1; k <= T; k++) {
	    minCoins[k] = INF_N_COINS;
	}

	//Step 2: Compute minCoins[i, k] for i > 0
	for (unsigned int i = 0; i < n; i++) {
	    for (unsigned int k = C[i]; k <= T; k++) {
	        if (minCoins[k - C[i]] < minCoins[k]) {
	            minCoins[k] = minCoins[k - C[i]] + 1;
	            lastCoin[k] = i;
	        }
	    }
	}
    if (minCoins[T] == INF_N_COINS) return false;

    //Step 3: Build the solution
    for (unsigned int i = 0; i < n; i++) {
        usedCoins[i] = 0;
    }
    int k = T;
    while (k > 0) {
        int nextID = lastCoin[k];
        usedCoins[nextID]++;
        k -= C[nextID];
    }

    return true;
}


/// TESTS ///
#include <gtest/gtest.h>

TEST(TP4_Ex2, hasChangeCanonical) {
    unsigned int C[] = {1,2,5,10};
    unsigned int n = 4;
    unsigned int usedCoins[4];

    EXPECT_EQ(changeMakingUnlimitedDP(C,n,13,usedCoins), true);
    EXPECT_EQ(usedCoins[0], 1);
    EXPECT_EQ(usedCoins[1], 1);
    EXPECT_EQ(usedCoins[2], 0);
    EXPECT_EQ(usedCoins[3], 1);

    EXPECT_EQ(changeMakingUnlimitedDP(C,n,38,usedCoins), true);
    EXPECT_EQ(usedCoins[0], 1);
    EXPECT_EQ(usedCoins[1], 1);
    EXPECT_EQ(usedCoins[2], 1);
    EXPECT_EQ(usedCoins[3], 3);

    EXPECT_EQ(changeMakingUnlimitedDP(C,n,10,usedCoins), true);
    EXPECT_EQ(usedCoins[0], 0);
    EXPECT_EQ(usedCoins[1], 0);
    EXPECT_EQ(usedCoins[2], 0);
    EXPECT_EQ(usedCoins[3], 1);
}

TEST(TP4_Ex2, hasChangeNonCanonical) {
    unsigned int C[] = {1,4,5};
    unsigned int n = 3;
    unsigned int usedCoins[3];

    EXPECT_EQ(changeMakingUnlimitedDP(C,n,6,usedCoins), true);
    EXPECT_EQ(usedCoins[0], 1);
    EXPECT_EQ(usedCoins[1], 0);
    EXPECT_EQ(usedCoins[2], 1);

    EXPECT_EQ(changeMakingUnlimitedDP(C,n,8,usedCoins), true);
    EXPECT_EQ(usedCoins[0], 0);
    EXPECT_EQ(usedCoins[1], 2);
    EXPECT_EQ(usedCoins[2], 0);

    EXPECT_EQ(changeMakingUnlimitedDP(C,n,7,usedCoins), true);
    EXPECT_EQ(usedCoins[0], 2);
    EXPECT_EQ(usedCoins[1], 0);
    EXPECT_EQ(usedCoins[2], 1);
}

TEST(TP4_Ex2, hasNoChange) {
    unsigned int C[] = {5,10};
    unsigned int n = 2;
    unsigned int usedCoins[2];

    EXPECT_EQ(changeMakingUnlimitedDP(C,n,1,usedCoins), false);
}