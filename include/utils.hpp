#ifndef UTILS_
#define UTILS_

#include <iostream>
#include <cmath>

namespace yLAB {

inline double determ(double a, double b, double c, double d) { //|a b|
    return a * d - c * b;                                      //|c d| = a * d - c * b
}

inline bool is_equal(double val1, double val2) {
    return std::fabs(val1 - val2) < std::numeric_limits<double>::epsilon();
}

};

#endif
