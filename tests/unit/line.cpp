#include <gtest/gtest.h>

#include "line.hpp"
#include "point.hpp"

using namespace yLAB;

TEST(Line_Testing, Initialization) {
    point_t pt1{1, -3, -5};
    point_t pt2{2, -3, 0};
    line_t ln1{pt1, pt2};
    
    EXPECT_TRUE( (ln1 == line_t{-5, 1, 1, 13}) );
    
    
    point_t pt3{0, 0, 0};
    point_t pt4{-1, 0, 0};
    line_t ln2{pt3, pt4};
    
    EXPECT_TRUE( (ln2 == line_t{0, 1, 1, 0}) );
    
    point_t pt5{-2, 0, 0};
    point_t pt6{0, -1, 0};
    line_t ln3{pt5, pt6};
    
    EXPECT_TRUE( (ln3 == line_t{0.5, 1, 1, 1}) );
    
    point_t pt7{1, 1, -1};
    point_t pt8{-9999, -99999, 4999};
    line_t ln4{pt7, pt8};
    
    EXPECT_TRUE( (ln4 == line_t{-9.5, 1, 1, 9.5}) );
    
    point_t pt9{10, 20, 300};
    point_t pt10{1, 0, 0};
    line_t ln5{pt9, pt10};
    
    EXPECT_TRUE( (ln5 == line_t{-320./9, 1, 1, 320./9}) );    
}

TEST(Line_Testing, Dirr_Vector) {
    line_t ln1{1, 0, 1, -5};
    vector_t dirr_vec1 = ln1.get_dirr_vec();

    EXPECT_TRUE((dirr_vec1 == vector_t{-1, 1, 1}));

    line_t ln2{1, 0, 0, -10543.54};
    vector_t dirr_vec2 = ln2.get_dirr_vec();

    EXPECT_TRUE((dirr_vec2 == vector_t{0, 1, 1}));
    
    line_t ln3{0, 1, 0, -5};
    vector_t dirr_vec3 = ln3.get_dirr_vec();

    EXPECT_TRUE((dirr_vec3 == vector_t{1, 0, 1}));

    line_t ln4{0, 0, 1, -5};
    vector_t dirr_vec4 = ln4.get_dirr_vec();

    EXPECT_TRUE((dirr_vec4 == vector_t{1, 1, 0}));
}

TEST(Line_Testing, Parallelism) {
    line_t ln1{1, 2, 3, 54.543};
    line_t ln2{2, 4, 6, 543};

    EXPECT_TRUE(ln1.is_parallel(ln2));
    
    line_t ln3{1, 0, 1, 0};
    line_t ln4{-1, -1, -1, -10.5};

    EXPECT_FALSE(ln3.is_parallel(ln4));
    
    line_t ln5{1, 0, 0, 0};
    line_t ln6{-54252435.53252352, 0, 0, 0};

    EXPECT_TRUE(ln5.is_parallel(ln6));
    
    line_t ln7{0, 1, 0, 54.543};
    line_t ln8{523.532, 0, 0, 543};

    EXPECT_FALSE(ln7.is_parallel(ln8));
    
    line_t ln9{1, -2, 3, 0};
    line_t ln10{-10.5, 21, -31.5, 0};

    EXPECT_TRUE(ln9.is_parallel(ln10));
}

TEST(Line_Testing, Containing_and_Random_Points) {
    static constexpr int tests_number = 100;
    line_t random_line{4351, -1.999999, 5534.523523, 15.5435};

    for (int count = 0; count < tests_number; ++count) {
        point_t tmp = random_line.get_point();
        EXPECT_TRUE(random_line.contains(tmp));
    }
}

TEST(Line_Testing, Intersection_Point) {

}

