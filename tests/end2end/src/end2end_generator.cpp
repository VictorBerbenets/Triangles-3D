#include <iostream>
#include <random>
#include <ctime>

#include "end2end_generator.hpp"
#include "point.hpp"
#include "plane.hpp"
#include "line.hpp"

namespace testing {

generator::generator(std::string file_name, u_int tests_number):
              file_name_{file_name},
              test_file_{file_name_},
              triangles_number_{tests_number} {
    generator_.seed(static_cast<u_int>(std::time(nullptr)));
}

void generator::generate_triangles() {
    test_file_ << triangles_number_ << ' ';

   // gener_type generator;
    //init_generator(generator);
    for (u_int count = 0; count < triangles_number_; ++count) {
        switch(static_cast<TriaOrientation>( random_uint() )) {
            case TriaOrientation::In_One_Plane:        one_plane_triangles();   break;
            case TriaOrientation::In_Different_Planes: diff_planes_triangles(); break;
            default: throw std::out_of_range{"generated number is out of valid case's"};
        }
    }
}

void generator::one_plane_triangles() {
     plane_t main_plane{random_point(), random_point(), random_point()};

    
}

void generator::diff_planes_triangles() {

}


generator::u_int generator::random_uint() noexcept {
    std::uniform_int_distribution<u_int> distribution(MIN_UINT_INDEX, MAX_UINT_INDEX);
    return distribution(generator_);
}

double generator::random_double() noexcept {
    std::uniform_real_distribution<> distribution(MIN_POINT_COORD, MAX_POINT_COORD);
    return distribution(generator_);
}
    
point_t generator::random_point() noexcept {
    return point_t{random_double(), random_double(), random_double()};
}

} // <--- namespace testing
