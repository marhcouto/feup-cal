//
// Created by marhc on 13/05/2021.
//
#include <iostream>
#include "gtest/gtest.h"

int main(int argc, char* argv[]) {

    // Tests
    std::cout << "Running all tests..." << std::endl;
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}

