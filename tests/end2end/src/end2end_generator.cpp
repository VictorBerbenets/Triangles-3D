#include <iostream>
#include <random>
#include <ctime>
#include <filesystem>
#include <fstream>

#include "end2end_generator.hpp"
#include "intersector.hpp"
#include "point.hpp"
#include "plane.hpp"
#include "line.hpp"

namespace testing {

generator::generator(std::string file_name, u_int tests_number,
                     int min_bound_box, int max_bound_box):
                        file_name_ {file_name},
                        triangles_number_ {tests_number},
                        tr_confines_ {min_bound_box, max_bound_box} {
    create_resources();
    generator_.seed(static_cast<u_int>(std::time(nullptr)));
};

void generator::create_resources() {
    using namespace std::filesystem;

    directory_entry resource_dir(dirs::res_dir);
    if (!resource_dir.is_directory()) {
        create_directory(dirs::res_dir);
        create_directory(dirs::tests_dir);
        create_directory(dirs::answ_dir);
    }
}

void generator::generate_points() {
    using distribution = std::uniform_real_distribution<>;

    std::ofstream test_file_(dirs::tests_dir + file_name_);
    test_file_ << triangles_number_ << ' ';

    for (u_int count1 = 0; count1 < triangles_number_; ++count1) {
        auto center = random_point();

        constexpr u_int POINTS_SET = TRIANGLES_POINTS*3;
        std::array<double, POINTS_SET> tria_coords {};
        for (u_int count2 = 0; count2 < POINTS_SET; ) {
            auto pt_coords = center.get_coords();
            for (u_int pt_count = 0; pt_count < 3; ++pt_count, ++count2) {
                tria_coords[count2] = random_float(pt_coords[pt_count] - tr_confines_[0], pt_coords[pt_count] + tr_confines_[1]);
            }
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

point_t generator::random_point() {
    return point_t {random_float(), random_float(), random_float()};
}

} // <--- namespace testing

