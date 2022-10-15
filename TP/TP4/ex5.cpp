#include "exercises.h"
#include <iostream>

unsigned long s_recursive(unsigned int n, unsigned int k) {
    if (k == 1 || n == k) return 1;
    return s_recursive(n - 1, k - 1) + k*s_recursive(n - 1, k);
}

unsigned long b_recursive(unsigned int n) {
    int sum = 0;
    for (unsigned int i = 1; i <= n; i++) {
        sum += s_recursive(n, i);
    }
    return sum;
}

unsigned long s_dynamic(unsigned int n, unsigned int k) {
    unsigned int len = n - k + 1;
    int strip[10000];

    for (unsigned int i = 0; i < len; i++) {
        strip[i] = 1;
    }

    for (int i = 2; i <= k; i++) { //Coluna a coluna
        for (unsigned int j = 1; j < len; j++) {
            strip[j] += i*strip[j - 1];
        }
    }
    return strip[len - 1];
}

unsigned long b_dynamic(unsigned int n) {
    int line[10000];

    for (unsigned int i = 1; i <= n; i++) {
        line[i - 1] = 1; //Fill the diagonal
        for (unsigned int k = i - 1; k > 1; k--){
            line[k - 1] = line[k - 2] + k*line[k - 1];
        }
    }

    unsigned int sum = 0;
    for (unsigned int k = 0; k < n; k++) {
        sum += line[k];
    }
    return sum;
}

/// TESTS ///
#include <gtest/gtest.h>

TEST(TP4_Ex5, testPartitioningRecursive) {
    EXPECT_EQ(1,s_recursive(3,3));
    EXPECT_EQ(3025,s_recursive(9,3));
    EXPECT_EQ(22827,s_recursive(10,6));

    EXPECT_EQ(5,b_recursive(3));
    EXPECT_EQ(203,b_recursive(6));
    EXPECT_EQ(1382958545,b_recursive(15));
}

TEST(TP4_Ex5, testPartitioningDynamic) {
    EXPECT_EQ(1,s_dynamic(3,3));
    EXPECT_EQ(25,s_dynamic(5,3));
    EXPECT_EQ(22827,s_dynamic(10,6));

    EXPECT_EQ(5,b_dynamic(3));
    EXPECT_EQ(203,b_dynamic(6));
    EXPECT_EQ(1382958545,b_dynamic(15));
}