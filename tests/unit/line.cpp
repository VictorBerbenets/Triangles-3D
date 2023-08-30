#include <gtest/gtest.h>

#include "line.hpp"
#include "point.hpp"

using namespace yLAB;

TEST(Line_Testing, Initialization) {
    point_t pt1{1, -3, -5};
    point_t pt2{2, -3, 0};
    line_t ln1{pt1, pt2};
    
    EXPECT_TRUE( (ln1 == line_t{-1, 0, -5, pt1}) );
    
    
    point_t pt3{0, 0, 0};
    point_t pt4{-1, 0, 0};
    line_t ln2{pt3, pt4};
    
    EXPECT_TRUE( (ln2 == line_t{1, 0, 0, pt3}) );
    
    point_t pt5{-2, 0, 0};
    point_t pt6{0, -1, 0};
    line_t ln3{pt5, pt6};
    
    EXPECT_TRUE( (ln3 == line_t{-2, 1, 0, pt5}) );
    
    point_t pt7{1, 1, -1};
    point_t pt8{-9999, -99999, 4999};
    line_t ln4{pt7, pt8};
    
    EXPECT_TRUE( (ln4 == line_t{10000, 100000, -5000, pt7}) );
    
    point_t pt9{10, 20, 300};
    point_t pt10{1, 0, 0};
    line_t ln5{pt9, pt10};
    
    EXPECT_TRUE( (ln5 == line_t{9, 20, 300, pt9}) );    
}


TEST(Line_Testing, Parallelism) {
    point_t pt1 {1, 2, 3};
    point_t pt2 {-1, -2, -3};
    
    line_t ln1{1, 2, 3, pt1};
    line_t ln2{2, 4, 6, pt2};
    EXPECT_TRUE(ln1.is_parallel(ln2));
    
    line_t ln3{0, 2, 3, pt1};
    line_t ln4{0, 4, 6, pt2};
    EXPECT_TRUE(ln3.is_parallel(ln4));
    
    line_t ln5{1, 1, 3, pt1};
    line_t ln6{2, 0, 6, pt2};
    EXPECT_FALSE(ln5.is_parallel(ln6));
    
}

TEST(Line_Testing, Intersection_Point) {

}

TEST(Line_Testing, Complanarity) {
    vector_t vec1{-3, 1, -5};
    vector_t vec2{1, 2, 4};
    vector_t vec3{3, -2, 4};

    EXPECT_TRUE(are_complanar(vec1, vec2, vec3));

}

