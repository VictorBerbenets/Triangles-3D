#include <gtest/gtest.h>
#include <cmath>

#include "point.hpp"
#include "utils.hpp"

using namespace yLAB;

TEST(Point_Tests, Validity) {
    
    point_t pt1{};
    point_t pt2{1, 2, NAN};
    point_t pt3{1, NAN, 2.5};
    point_t pt4{NAN, 1, 1};
    point_t pt5{NAN, NAN, NAN};

    EXPECT_FALSE(pt1.is_valid());
    EXPECT_FALSE(pt2.is_valid());
    EXPECT_FALSE(pt3.is_valid());
    EXPECT_FALSE(pt4.is_valid());
    EXPECT_FALSE(pt5.is_valid());
    
    point_t pt6{1, 2, -1};
    EXPECT_TRUE(pt6.is_valid());
}

TEST(Point_Tests, Comparing) {
    point_t pt1{};
    point_t pt2{-1, 2, NAN};
    point_t pt3{-1, NAN, 2.5};
    point_t pt4{NAN, 1, 1};
    point_t pt5{NAN, NAN, NAN};

    EXPECT_FALSE(pt1 == pt2);
    EXPECT_FALSE(pt1 == pt3);
    EXPECT_FALSE(pt3 == pt2);
    EXPECT_FALSE(pt3 == pt4);
    EXPECT_FALSE(pt4 == pt5);
    EXPECT_FALSE(pt2 == pt5);
    
    point_t pt6{1.5, 1.14888814, -20.5435};
    point_t pt7{1.5, 1.14888814, -20.5435};
    EXPECT_TRUE(pt6 == pt7);

    point_t pt8{0, 0, -20.5435};
    point_t pt9{0, 0, -20.5435};
    EXPECT_TRUE(pt8 == pt9);

    point_t pt10{-10000.777, 0, -20.5435};
    point_t pt11{-10000.777, 0, -20.5435};
    EXPECT_TRUE(pt10 == pt11);
}

