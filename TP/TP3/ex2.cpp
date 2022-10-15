#include "exercises.h"
#include <iostream>
#include <math.h>

int maxSubsequenceDCRec(int A[], int start, int finish, int &i, int &j) {

    //Base step
    if (start == finish) {
        i = start;
        j = finish;
        return A[i];
    }

    //Recursive step
    int start2 = (int)std::round((start + finish) / 2.0);
    int finish2 = start2 - 1;
    int i1, j1, i2, j2;
    int bestSum1 = maxSubsequenceDCRec(A, start2, finish, i1, j1); //1st
    int bestSum2 = maxSubsequenceDCRec(A, start, finish2, i2, j2); //2nd

    //3rd
    int sum = A[finish2];
    int bestCrossingSum1 = A[finish2];
    int bestCrossingI = finish2;

    for (int k = finish2 - 1; k >= start; k--) {
        sum += A[k];
        if (sum > bestCrossingSum1) {
            bestCrossingSum1 = sum;
            bestCrossingI = k;
        }
    }

    sum = A[start2];
    int bestCrossingSum2 = A[start2];
    int bestCrossingJ = start2;

    for (int k = start2 + 1; k <= finish; k++) {
        sum += A[k];
        if (sum > bestCrossingSum2) {
            bestCrossingSum2 = sum;
            bestCrossingJ = k;
        }
    }
    int bestCrossingSum = bestCrossingSum1 + bestCrossingSum2;

    //Decision
    if (bestCrossingSum > bestSum1 && bestCrossingSum > bestSum2) {
        i = bestCrossingI;
        j = bestCrossingJ;
        return bestCrossingSum;
    }
    else if (bestSum1 > bestSum2) {
        i = i1;
        j = j1;
        return bestSum1;
    }
    else {
        i = i2;
        j = j2;
        return bestSum2;
    }
}

int maxSubsequenceDC(int A[], unsigned int n, int &i, int &j) {
    if (n == 0) return 0;
    return maxSubsequenceDCRec(A, 0, n - 1, i, j);
}

/// TESTS ///
#include <gtest/gtest.h>

TEST(TP3_Ex2, testMaxSubsequence) {
    int A1[] = {1, 2, 3, 4};
    unsigned int n1 = 4;
    int i, j;
    EXPECT_EQ(maxSubsequenceDC(A1,n1,i,j), 10);
    EXPECT_EQ(i, 0);
    EXPECT_EQ(j, 3);

    int A2[] = {2, -1, -3, 4};
    unsigned int n2 = 4;
    EXPECT_EQ(maxSubsequenceDC(A2,n2,i,j), 4);
    EXPECT_EQ(i, 3);
    EXPECT_EQ(j, 3);

    int A3[] = {2, -1, 3, 4};
    unsigned int n3 = 4;
    EXPECT_EQ(maxSubsequenceDC(A3,n3,i,j), 8);
    EXPECT_EQ(i, 0);
    EXPECT_EQ(j, 3);

    int A4[] = {-2, 1, -3, 4, -1, 2, 1, -5, 4};
    unsigned int n4 = 9;
    EXPECT_EQ(maxSubsequenceDC(A4,n4,i,j), 6);
    EXPECT_EQ(i, 3);
    EXPECT_EQ(j, 6);
}