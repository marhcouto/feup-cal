
#include "exercises.h"

#include <vector>
#include <iostream>
#include <fstream>
#include <algorithm>
#include <sstream>

int editDistance(std::string pattern, std::string text) {

    int distances[pattern.size() + 1][text.size() + 1];

    for (unsigned int i = 0; i <= pattern.size(); i++) {
        for (unsigned int j = 0; j <= text.size(); j++) {
            if (i == 0)
                distances[i][j] = j;
            else if (j == 0)
                distances[i][j] = i;
            else if (pattern.at(i - 1) == text.at(j - 1))
                distances[i][j] = distances[i - 1][j - 1];
            else
                distances[i][j] = std::min(std::min(distances[i][j - 1], distances[i - 1][j]), distances[i - 1][j - 1]) + 1;
        }
    }
    return distances[pattern.size()][text.size()];
}

float numApproximateStringMatching(std::string filename, std::string toSearch) {
    float sum = 0.0;
    float counter = 0.0;
    std::string word;
    std::ifstream f(filename);
    while(!f.eof()) {
        f >> word;
        sum += editDistance(toSearch, word);
        counter += 1.0;
    }
    return sum / counter;
}

/// TESTS ///
#include <gtest/gtest.h>

TEST(TP10_Ex2, testEditDistance) {
    EXPECT_EQ(3, editDistance("abcdefghijkl", "bcdeffghixkl"));
    EXPECT_EQ(3, editDistance("bcdeffghixkl", "abcdefghijkl"));

    EXPECT_EQ(1, editDistance("", "b"));
    EXPECT_EQ(1, editDistance("a", ""));
    EXPECT_EQ(1, editDistance("a", "b"));
    EXPECT_EQ(0, editDistance("", ""));
    EXPECT_EQ(0, editDistance("a", "a"));
}

#define REL_PATH std::string("../TP10/") // relative path to the tests

TEST(TP10_Ex2, testNumApproximateStringMatching) {
    const float delta = 0.01;

    const float expected1 = 7.76;
    float dist1 = numApproximateStringMatching(REL_PATH + "text1.txt", "estrutur");
    ASSERT_LE(expected1-delta, dist1);
    ASSERT_GE(expected1+delta, dist1);

    const float expected2 = 7.49;
    float dist2 = numApproximateStringMatching(REL_PATH + "text2.txt", "estrutur");
    ASSERT_LE(expected2-delta, dist2);
    ASSERT_GE(expected2+delta, dist2);
}