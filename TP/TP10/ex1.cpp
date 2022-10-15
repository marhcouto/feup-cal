#include "exercises.h"

#include <vector>
#include <iostream>
#include <fstream>

int kmpMatcher(std::string pattern, std::string text) {
    if (text.empty() || pattern.empty()) return 0;
    if (text.size() < pattern.size()) return 0;

    int count = 0;
    int lps[pattern.size()];
    lps[0] = 0;
    int k = 0;

    // Basically, lps[i] is the length of the biggest prefix that has a suffix that is the same,
    //the string pattern[0..i]
    // This will be the value it can come back to in the next cycle, so to avoid discarding the whole string
    for (unsigned int i = 1; i < pattern.size(); i++) {
        while(k > 0 && pattern.at(k) != pattern.at(i))
            k = lps[k - 1];
        if (pattern.at(k) == pattern.at(i))
            k++;
        lps[i] = k;
    }

    int q = 0;
    for (char i : text) {
        while(q > 0 && pattern.at(q) != i)
            q = lps[q - 1];
        if (pattern.at(q) == i)
            q++;
        if (q >= pattern.size() - 1) { // >= for a 1 letter pattern
            count++;
            q = lps[q];
        }
    }
    return count;
}

int numStringMatching(std::string filename, std::string toSearch) {
    int count = 0;
    std::string line;
    std::ifstream f(filename);
    while (std::getline(f, line))
        count += kmpMatcher(toSearch, line);
    return count;
}

/// TESTS ///
#include <gtest/gtest.h>

TEST(TP10_Ex1, testKmpMatcher) {
    EXPECT_EQ(3, kmpMatcher("aabaa", "aaabaabaacaabaa"));

    EXPECT_EQ(0, kmpMatcher("", "a"));
    EXPECT_EQ(1, kmpMatcher("a", "a"));
}

#define REL_PATH std::string("../TP10/") // relative path to the tests

TEST(TP10_Ex1, testNumStringMatching) {
    int num1 = numStringMatching(REL_PATH + "text1.txt", "estrutura de dados");
    EXPECT_EQ(3, num1);

    int num2=numStringMatching(REL_PATH +"text2.txt", "estrutura de dados");
    EXPECT_EQ(2,num2);
}