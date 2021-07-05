#include "exercises.h"

int maxSubsequence(int A[], unsigned int n, unsigned int &i, unsigned int &j) {
    int maxSum = 0;
    for (unsigned int i1 = 0; i1 < n; i1++) {
        for (unsigned int j1 = i1; j1 < n; j1++) {
            int sum = 0;
            for (unsigned int k = i1; k <= j1; k++)
                sum += A[k];
            if (sum > maxSum) {
                i = i1; j = j1;
                maxSum = sum;
            }
        }
    }
    return maxSum;
}


/// TESTS ///
#include <gtest/gtest.h>

TEST(TP1_Ex2, maxSubsequence) {
    int A[] = {-2, 1, -3, 4, -1, 2, 1, -5, 4};
    unsigned int n = 9;
    unsigned int i, j;
    EXPECT_EQ(maxSubsequence(A,n,i,j), 6);
    EXPECT_EQ(i, 3);
    EXPECT_EQ(j, 6);
}