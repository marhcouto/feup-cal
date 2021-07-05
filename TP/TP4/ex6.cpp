#include "exercises.h"
#include <cmath>
#include <chrono>
#include <iostream>
#include <fstream>

int maxSubsequenceDPRec(int A[], unsigned int k, unsigned int& i, unsigned int& j, int bestSum[], int bestI[], int bestJ[]) {

    if (bestI[k] != -1) { // Already processed
        i = bestI[k];
        j = bestJ[k];
        return bestSum[k];
    }
    if (k == 0) { // Ending
        bestI[k] = 0;
        bestJ[k] = 0;
        return (bestSum[k] = A[k]);
    }
    else if (maxSubsequenceDPRec(A, k - 1, i, j, bestSum, bestI, bestJ) >= 0) { // If sum is positive
        bestI[k] = bestI[k - 1];
        bestJ[k] = k;
        return bestSum[k] = A[k] + bestSum[k - 1];
    }
    else { // Reset
        bestI[k] = k;
        bestJ[k] = k;
        return bestSum[k] = A[k];
    }
}

int maxSubsequenceDP(int A[], unsigned int n, unsigned int &i, unsigned int &j) {

    if (n == 0) return 0;

    //TOP DOWN

    int bestSums[10000];
    int bestI[10000];
    int bestJ[10000];

    for (unsigned int k = 0; k < n; k++)
        bestI[k] = -1;

    int bestSum = maxSubsequenceDPRec(A, n - 1, i, j, bestSums, bestI, bestJ);
    i = bestI[n - 1];
    j = bestJ[n - 1];

    for (unsigned int k = 0; k < n - 1; k++) {
        if (bestSums[k] > bestSum) {
            bestSum = bestSums[k];
            i = bestI[k];
            j = bestJ[k];
        }
    }
    return bestSum;

    //BOTTOM UP
    /*
    i = 0, j = 0;
    int currSum = A[k], bestSum = A[k];

    for (unsigned int k = 0; k < n; k++) {

        //1-Check if the sum is faulty
        if (A[k] > currSum + A[k]) {
            i = k;
            currSum = A[k];
        }
        else
            currSum = currSum + A[k];

        //Update best
        if (currSum > bestSum) {
            bestSum = currSum;
            j = k;
        }
    }
    return bestSum;*/
}

int maxSubsequenceBF(int A[], unsigned int n, unsigned int &i, unsigned int &j) {
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

int maxSubsequenceDC(int A[], unsigned int n, unsigned int &i, unsigned int &j) {
    if (n == 0) return 0;
    return maxSubsequenceDCRec(A, 0, n - 1, reinterpret_cast<int &>(i), reinterpret_cast<int &>(j));
}

void testPerformanceMaxSubsequence() {
    std::ofstream fs;
    fs.open("../TP4/times.csv", std::ios::trunc);
    srand(time(NULL));
    for (long long int n = 1000; n < 50000; n += 1000) {
        unsigned int i, j;
        int* A = new int[n];
        for (unsigned int i = 0; i < n; i++)
            A[i] = rand() % 10000 - 4000;
        auto moment1 = std::chrono::high_resolution_clock::now();
        if (n < 2000)
            maxSubsequenceBF(A, n, i, j);
        auto moment2 = std::chrono::high_resolution_clock::now();
        maxSubsequenceDC(A, n, i, j);
        auto moment3 = std::chrono::high_resolution_clock::now();
        maxSubsequenceDP(A, n, i, j);
        auto moment4 = std::chrono::high_resolution_clock::now();
        auto duration1 = std::chrono::duration_cast<std::chrono::microseconds>(moment2 - moment1);
        auto duration2 = std::chrono::duration_cast<std::chrono::microseconds>(moment3 - moment2);
        auto duration3 = std::chrono::duration_cast<std::chrono::microseconds>(moment4 - moment3);
        fs << duration1.count() << ',' << duration2.count() << ',' << duration3.count() << std::endl;
        delete A;
    }
    fs.close();
}

/// TESTS ///
#include <gtest/gtest.h>

TEST(TP4_Ex6, testMaxSubsequence) {
    int A1[] = {1, 2, 3, 4};
    unsigned int n1 = 4;
    unsigned int i, j;
    EXPECT_EQ(maxSubsequenceDP(A1,n1,i,j), 10);
    EXPECT_EQ(i, 0);
    EXPECT_EQ(j, 3);

    int A2[] = {2, -1, -3, 4};
    unsigned int n2 = 4;
    EXPECT_EQ(maxSubsequenceDP(A2,n2,i,j), 4);
    EXPECT_EQ(i, 3);
    EXPECT_EQ(j, 3);

    int A3[] = {2, -1, 3, 4};
    unsigned int n3 = 4;
    EXPECT_EQ(maxSubsequenceDP(A3,n3,i,j), 8);
    EXPECT_EQ(i, 0);
    EXPECT_EQ(j, 3);

    int A4[] = {-2, 1, -3, 4, -1, 2, 1, -5, 4};
    unsigned int n4 = 9;
    EXPECT_EQ(maxSubsequenceDP(A4,n4,i,j), 6);
    EXPECT_EQ(i, 3);
    EXPECT_EQ(j, 6);
    testPerformanceMaxSubsequence();

}