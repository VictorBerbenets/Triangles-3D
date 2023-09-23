#include <gtest/gtest.h>

#include "vector.hpp"
#include "utils.hpp"

using namespace yLAB;

TEST(Vector, Operators) {
    vector_t ml_vec1{1, 2, 3};
    ml_vec1 *= 15;
    ASSERT_EQ(ml_vec1, (vector_t{15, 30, 45}));

    vector_t div_vec2{60, 30, 20};
    div_vec2 /= 15;
    ASSERT_EQ(div_vec2, (vector_t{4, 2, 4./3}));
    
    vector_t sum_vec3{1, 2, 34};
    vector_t sum_vec4{-532.5, 32, 3};
    auto sum_result = sum_vec3 + sum_vec4;
    ASSERT_EQ(sum_result, (vector_t{-531.5, 34, 37}));
    
    vector_t sub_vec4{1, 2, 3};
    vector_t sub_vec5{1, 2, 3};
    auto sub_result = sub_vec4 - sub_vec5;
    ASSERT_EQ(sub_result, (vector_t{0, 0, 0}));
    
    vector_t div_vec5{300, 200, 3};
    double divider = 150;
    auto div_result = div_vec5 / divider;
    ASSERT_EQ(div_result, (vector_t{2, 4./3, 1./50}));
    
    vector_t mul_vec5{-1, -1, -10};
    double coeff = 25;
    auto mul_result = mul_vec5 * coeff;
    ASSERT_EQ(mul_result, (vector_t{-25, -25, -250}));
}

TEST(Vector, Module) {
    vector_t vec1{1, 2, 3};
    EXPECT_TRUE( ( are_equal(vec1.get_module(), std::sqrt(14)) ) );
    
    vector_t vec2{1, 1, 0};
    EXPECT_TRUE( ( are_equal(vec2.get_module(), std::sqrt(2)) ) );
    
    vector_t vec3{-1, -2, -1};
    EXPECT_TRUE( ( are_equal(vec3.get_module(), std::sqrt(6)) ) );
    
    vector_t vec4{1, -2, -100};
    EXPECT_TRUE( ( are_equal(vec4.get_module(), std::sqrt(10005)) ) );
    
    vector_t vec5{0, 0, 0};
    EXPECT_TRUE( ( are_equal(vec5.get_module(), std::sqrt(0)) ) );
}

TEST(Vector, Null_And_Valid) {
    vector_t vec1{0, 0, 0};
    EXPECT_TRUE(vec1.is_null());

    vector_t vec2{NAN, 0, 0};
    EXPECT_FALSE(vec2.is_valid());
    
    vector_t vec3{0, NAN, 0};
    EXPECT_FALSE(vec3.is_valid());
    
    vector_t vec4{0, 0, NAN};
    EXPECT_FALSE(vec4.is_valid());
    
    vector_t vec5{NAN, NAN, NAN};
    EXPECT_FALSE(vec5.is_valid());
    
    vector_t vec6{NAN, 1, -543542.5};
    EXPECT_FALSE(vec5.is_null());

    vector_t vec7{1, 1, 1};
    EXPECT_TRUE(vec7.is_valid());
}

