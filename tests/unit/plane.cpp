#include <gtest/gtest.h>

#include "plane.hpp"

using namespace yLAB;

TEST(Plane, Parallelism) {
    plane_t plane1{point_t{0, 0, 1}, point_t{0, 1, 0}, point_t{0, 1, 1}};
    plane_t plane2{point_t{0, 0, 1}, point_t{0, 1, 0}, point_t{0, 1, 1}};
    EXPECT_TRUE(plane1.is_parallel(plane2));
    
    plane_t plane3{point_t{0, 0, 0}, point_t{0, 1, 0}, point_t{1, 1, 0}};
    plane_t plane4{point_t{1, 1, 1}, point_t{0, 1, 1}, point_t{0, 0, 1}};
    EXPECT_TRUE(plane3.is_parallel(plane4));
    
    plane_t plane5{point_t{0, 0, 1}, point_t{0, 1, 0}, point_t{0, 0, 0}};
    plane_t plane6{point_t{1, 1, 1}, point_t{0, 1, 1}, point_t{0, 0, 1}};
    EXPECT_FALSE(plane5.is_parallel(plane6));
}

TEST(Plane, PlanesPoints) {
    plane_t plane1{point_t{0, 0, 0}, point_t{-1, 1, 0}, point_t{0, -1, 0}};
    EXPECT_TRUE(plane1.contains({0, 0, 0}));
    EXPECT_TRUE(plane1.contains({0, 3452435.5325, 0}));
    EXPECT_TRUE(plane1.contains({-5235.32532, 5325, 0}));
    EXPECT_FALSE(plane1.contains({1, 1, 1}));
    EXPECT_FALSE(plane1.contains({0, 0, -10532.532}));

    auto plane_pt = plane1.get_plane_point();
    EXPECT_TRUE(plane1.contains(plane_pt));
}

