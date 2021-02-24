#include "exercises.h"

static std::vector<Activity> best;

bool Activity::operator==(const Activity &a2) const {
    return start == a2.start && finish == a2.finish;
}

bool Activity::overlaps(const Activity &a2) const {
    return (start < a2.finish) && (a2.start < finish);
}

void actSelRec(const std::vector<Activity> &left, std::vector<Activity>::iterator it, std::vector<Activity> chosen) {
    if (it == left.end()) {
        if (chosen.size() > best.size()) {
            best = chosen;
        }
        return;
    }

    actSelRec(left, it+1, chosen);

    for (const Activity &a : chosen) {
        if (a.overlaps(*it))
            return;
    }
    chosen.push_back(*it);
    actSelRec(left, it+1, chosen);
}

std::vector<Activity> activitySelectionBacktracking(std::vector<Activity> A) {
    best.clear();
    actSelRec(A, A.begin(), {});
    return best;
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