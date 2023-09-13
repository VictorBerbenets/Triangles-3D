#include <iostream>
#include <random>
#include <ctime>

#include "end2end_generator.hpp"
#include "intersector.hpp"
#include "point.hpp"
#include "plane.hpp"
#include "line.hpp"

namespace testing {

generator::generator(std::string file_name, u_int tests_number, double space_confine, u_int min_tria_side, u_int max_tria_side):
                        file_name_ {file_name},
                        test_file_ {file_name},
                        triangles_number_ {tests_number},
                        sp_confine_ {space_confine},
                        tr_confines_ {min_tria_side, max_tria_side} {};


void generate_triangles() {

}

} // <--- namespace testing
