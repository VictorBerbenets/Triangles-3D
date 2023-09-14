#include <iostream>
#include <utility>
#include <stdexcept>

#include "end2end_generator.hpp"

auto get_data(int argc, const char* argv[]) {
    using gener = testing::generator;
    using size_type = std::size_t;

    std::cout << "ARGC = " << argc << std::endl;
    size_type triangles_number = std::stoi(argv[2]);
    if (triangles_number < 0) {
        throw std::invalid_argument{"triangles number must be positive value"};
    }
    if (argc == 3) {
        return std::array<size_type, 3>{triangles_number, gener::MAX_POINT_COORD, gener::MAX_POINT_COORD};
    } else if (argc == 4) {
        size_type min_point_coord = std::stoi(argv[3]);
        return std::array<size_type, 3>{triangles_number, min_point_coord, gener::MAX_POINT_COORD};
    } else if (argc == 5) {
        size_type min_point_coord = std::stoi(argv[3]);
        size_type max_point_coord = std::stoi(argv[4]);
        return std::array<size_type, 3>{triangles_number, min_point_coord, max_point_coord};
    } else {
        throw std::invalid_argument{"error: expected two main parameters: file name, number of triangles, and two optional params"};
    }

}

int main(int argc, const char* argv[]) {
    using namespace testing;
    if (argc < 3) {
        throw std::invalid_argument{"error: expected two main parameters: file name, number of triangles, and two optional params"};
    }
    std::string file_name = argv[1];
    auto [trias_number, min_point_coord, max_point_coord] = get_data(argc, argv);

    generator gen(file_name, trias_number, min_point_coord, max_point_coord);
    gen.generate_points();
}
