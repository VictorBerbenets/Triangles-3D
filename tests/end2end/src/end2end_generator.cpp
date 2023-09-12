#include <iostream>
#include <random>
#include <ctime>

#include "end2end_generator.hpp"

namespace testing {

void generator::generate_triangles() const {
    gener_type generator;
    init_generator(generator);
    
    switch(static_cast<TriaOrientation>(random_uint(generator))) {
        case TriaOrientation::In_One_Plane:        one_plane_triangles();   break;
        case TriaOrientation::In_Different_Planes: diff_planes_triangles(); break;
        default: std::cout << "something wrong" << std::endl;
    }
}

void generator::one_plane_triangles() const {
    
}

void generator::diff_planes_triangles() const {

}

void generator::init_generator(gener_type& generator) const {
      const u_int seed = static_cast<u_int>(std::time(nullptr));
      generator.seed(seed);
}

generator::u_int generator::random_uint(gener_type& generator) const {
    std::uniform_int_distribution<u_int> distribution(0, 1);
    return distribution(generator);
}

double generator::random_double(gener_type& generator) const {
    std::uniform_real_distribution<> distribution(MIN_POINT_COORD, MAX_POINT_COORD);
    return distribution(generator);
}

} // <--- namespace testing
