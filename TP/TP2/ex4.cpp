#include "exercises.h"
#include <iostream>

bool Activity::operator==(const Activity &a2) const {
    return start == a2.start && finish == a2.finish;
}

bool Activity::overlaps(const Activity &a2) const {
    return (start < a2.finish) && (a2.start < finish);
}

void activitySelectionBacktrackingRec(std::vector<Activity> A, std::vector<Activity> currentSol, unsigned int currIndex,
                                      std::vector<Activity>& bestSol) {

    if (currIndex == A.size()) {
        if (currentSol.size() > bestSol.size())
            bestSol = currentSol;
        return;
    }
    bool overlaps = false;
    for (auto act : currentSol) {
        if (act.overlaps(A[currIndex]))
            overlaps = true;
    }
    // Add activity
    activitySelectionBacktrackingRec(A, currentSol, currIndex+1, bestSol);

    // Don't
    if (!overlaps) {
        currentSol.push_back(A[currIndex]);
        activitySelectionBacktrackingRec(A, currentSol, currIndex+1, bestSol);
    }
}

std::vector<Activity> activitySelectionBacktracking(std::vector<Activity> A) {
    std::vector<Activity> bestSol = {};
    activitySelectionBacktrackingRec(A, bestSol, 0, bestSol);
    return bestSol;
}

/// TESTS ///
#include <gtest/gtest.h>
#include <gmock/gmock.h>

bool noOverlaps(const std::vector<Activity> &acts) {
    for(unsigned int i = 0; i < acts.size(); i++) {
        Activity A1 = acts.at(i);
        for(unsigned int j = i + 1; j < acts.size(); j++) {
            Activity A2 = acts.at(j);
            if(A1.overlaps(A2)) return false;
        }
    }
    return true;
}

TEST(TP2_Ex4, activityScheduling) {
    std::vector<Activity> A = {{10,20}, {30, 35}, {5, 15}, {10, 40}, {40, 50}};
    std::vector<Activity> V = activitySelectionBacktracking(A);
    EXPECT_EQ(V.size(), 3 );
    EXPECT_EQ(noOverlaps(V), true);
}