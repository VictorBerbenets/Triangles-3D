#ifndef UTILS_
#define UTILS_

#include <cmath>

#include "vector.hpp"
#include "point.hpp"

namespace yLAB {

inline double determ(double a, double b, double c, double d) { //|a b|
    return a * d - c * b;                                      //|c d| = a * d - c * b
}

inline bool are_equal(double val1, double val2) {
    static constexpr double EPSILON = 1e-6;
    return std::fabs(val1 - val2) < EPSILON;
}

inline bool is_zero(double expr) {
    return are_equal(expr, 0);
}

inline vector_t calc_vects_product(const vector_t& vec1, const vector_t& vec2) {
    //    -> -> ->
    // ||  i  j  k ||   ->                    ->                    ->
    // || a1 b1 c1 || = i * (b1*c2 - c1*b2) - j * (a1*c2 - a2*c1) + k * (a1*b2 - b1*a2)
    // || a2 b2 c2 ||
    return { determ(vec1[1], vec1[2], vec2[1], vec2[2]),
            -determ(vec1[0], vec1[2], vec2[0], vec2[2]),
             determ(vec1[0], vec1[1], vec2[0], vec2[1]), };
}
inline bool are_coplanar(const vector_t& vec1, const vector_t& vec2, const vector_t& vec3) {
    // | a1 a2 a3 |
    // | b1 b2 b3 | = 0
    // | c1 c2 c3 |
    return is_zero( vec1[0] * determ(vec2[1], vec2[2], vec3[1], vec3[2]) -
                    vec1[1] * determ(vec2[0], vec2[2], vec3[0], vec3[2]) +
                    vec1[2] * determ(vec2[0], vec2[1], vec3[0], vec3[1]) );
}

inline bool is_null_vector(const vector_t& vector) {
    return is_zero(vector[0]) &&
           is_zero(vector[1]) &&
           is_zero(vector[2]);
}

inline double scalar_multiply(const vector_t& vec1, const vector_t& vec2) {            
    return vec1[0] * vec2[0] + vec1[1] * vec2[1] + vec1[2] * vec2[2];
}


} // <--- namespace yLAB

#endif
