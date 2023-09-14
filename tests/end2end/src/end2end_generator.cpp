#include <iostream>
#include <random>
#include <ctime>

#include "end2end_generator.hpp"
#include "intersector.hpp"
#include "point.hpp"
#include "plane.hpp"
#include "line.hpp"

namespace testing {

generator::generator(std::string file_name, u_int tests_number,
                     /*double space_confine,*/ int min_tria_side, int max_tria_side):
                        file_name_ {file_name},
                        test_file_ {file_name},
                        triangles_number_ {tests_number},
                        //sp_confine_ {space_confine},
                        tr_confines_ {min_tria_side, max_tria_side} {
    generator_.seed(static_cast<u_int>(std::time(nullptr)));
};

void generator::generate_points() {
    using distribution = std::uniform_real_distribution<>;
    

    test_file_ << triangles_number_ << ' ';
    //std::vector<point_t> pts{};
    //pts.reserve(TRIANGLES_POINTS * triangles_number);
    for (u_int count1 = 0; count1 < triangles_number_; ++count1) {
        std::cout << "COUNT1 = " << count1 << std::endl;
        auto center = random_point();
        std::cout << "CENTER POINT: " << center << std::endl;
        constexpr u_int POINTS_SET = TRIANGLES_POINTS*3;
        std::cout << "SET = " << POINTS_SET << std::endl;
        std::array<double, POINTS_SET> tria_coords {};
        for (u_int count2 = 0; count2 < 9; ++count2) {
        std::cout << "COUNT2 = " << count2 << std::endl;
            auto pt_coords = center.get_coords();
            //tria_coords[count2] = random_float(pt_coords[count2] - tr_confines_[0], pt_coords[count2] + tr_confines_[1]);
            tria_coords[count2] = random_float();
            std::cout << "RANDOM = " << tria_coords[count2] << std::endl; 
        }
        for (u_int count3 = 0; count3 < POINTS_SET; ++count3) {
            test_file_ << tria_coords[count3] << ' ';
        }
    }
}

double generator::random_float(double min_val, double max_val) {
    std::uniform_real_distribution<> distr(min_val, max_val);
    return distr(generator_);
}

generator::u_int generator::random_uint(u_int min_val, u_int max_val) {
    std::uniform_int_distribution<> distr(min_val, max_val);
    return distr(generator_);
}

point_t generator::random_point() {
    return point_t {random_float(), random_float(), random_float()};
}

} // <--- namespace testing
