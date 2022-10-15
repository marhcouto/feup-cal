#include "exercises.h"
#include <algorithm>
#include <vector>

double minimumAverageCompletionTime(std::vector<unsigned int> tasks, std::vector<unsigned int> &orderedTasks) {
    std::sort(tasks.begin(), tasks.end());
    orderedTasks = tasks;
    float avgTime = 0.0, additive = 0.0;
    for (auto task : orderedTasks) {
        additive += task;
        avgTime += additive;
    }
    avgTime /= orderedTasks.size();
    return avgTime;
}

/// TESTS ///
#include <gtest/gtest.h>
#include <gmock/gmock.h>

TEST(TP1_Ex7, taskOrdering) {
    std::vector<unsigned int> tasks = {15, 8, 3, 10};
    std::vector<unsigned int> orderedTasks;
    double averageTime = minimumAverageCompletionTime(tasks, orderedTasks);
    EXPECT_EQ(orderedTasks.size(), 4 );
    ASSERT_NEAR(averageTime, 17.75, 0.00001);
    ASSERT_THAT(orderedTasks,  ::testing::ElementsAre(3,8,10,15));
}