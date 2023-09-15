#include <iostream>
#include <utility>
#include <stdexcept>

#include "end2end_generator.hpp"

auto get_data(int argc, const char* argv[]) {
    using gener = testing::generator;
    using size_type = std::size_t;

    int triangles_number = std::stoi(argv[2]);
    if (triangles_number < 0) {
        throw std::invalid_argument{"triangles number must be positive value"};
    }
    if (argc == 3) {
        return std::array<int, 3>{triangles_number, gener::MIN_BOUND_BOX, gener::MAX_BOUND_BOX};
    } else if (argc == 4) {
        int min_box = std::stoi(argv[3]);
        min_box = min_box == 0 ? gener::MIN_BOUND_BOX : min_box;
        return std::array<int, 3>{triangles_number, min_box, gener::MAX_BOUND_BOX};
    } else if (argc == 5) {
        int min_box = std::stoi(argv[3]);
        int max_box = std::stoi(argv[4]);
        min_box = min_box == 0 ? gener::MIN_BOUND_BOX : min_box;
        max_box = max_box == 0 ? gener::MAX_BOUND_BOX : max_box;
        return std::array<int, 3>{triangles_number, min_box, max_box};
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
    auto [trias_number, min_box, max_box] = get_data(argc, argv);

    generator gen(file_name, trias_number, min_box, max_box);
    gen.generate_points();
}
