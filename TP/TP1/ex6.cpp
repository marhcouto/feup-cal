#include "exercises.h"
#include <algorithm>

bool Activity::operator==(const Activity &a2) const {
    return start == a2.start && finish == a2.finish;
}

bool Activity::operator<(const Activity &a2) const {
    return finish < a2.finish;
}

std::vector<Activity> earliestFinishScheduling(std::vector<Activity> A) {
    Activity last(0, 0);
    std::sort(A.begin(), A.end());
    std::vector<Activity> res;
    for (auto act : A) {
        if (act.start < last.finish) continue;
        res.push_back(act);
        last = act;
    }
    return res;
}

/// TESTS ///
#include <gtest/gtest.h>
#include <gmock/gmock.h>

TEST(TP1_Ex6, activityScheduling) {
    std::vector<Activity> A = {{10,20}, {30, 35}, {5, 15}, {10, 40}, {40, 50}};
    std::vector<Activity> V = earliestFinishScheduling(A);
    EXPECT_EQ(V.size(), 3 );
    ASSERT_THAT(V,  ::testing::ElementsAre(Activity(5, 15), Activity(30, 35), Activity(40, 50)));
}