#include <iostream>

#include <cmath>
#include <chrono>

#include "admin.hpp"

auto give_data(std::istream& stream) {
    using namespace yLAB;

    using data_type   = admin::data_type;
    using size_type   = admin::size_type;

    size_type data_size{};
    stream >> data_size;
    if (!stream.good()) {
        throw std::runtime_error{"data size reading error\n"};
    }

    double max_hlf_side = 0;

    std::vector<data_type> data{};
    data.reserve(data_size);

    std::vector<double> tmp_points{};
    tmp_points.reserve(admin::SET_POINTS_SIZE); //  processing of input data

    for (size_type count = 0; count < data_size; ++count) {
        double tmp_value{};
        for (size_type points_number = 0; points_number < admin::SET_POINTS_SIZE; ++points_number) {
            stream >> tmp_value;
            if (std::greater<double>{}(std::fabs(tmp_value), max_hlf_side)) {
                max_hlf_side = std::fabs(tmp_value);
            }
            tmp_points.push_back(tmp_value);
        }
        triangle_t tria = { {tmp_points[0], tmp_points[1], tmp_points[2]},
                            {tmp_points[3], tmp_points[4], tmp_points[5]},
                            {tmp_points[6], tmp_points[7], tmp_points[8]} };
        data.emplace_back(tria, spaceBreaking::AABB{tria}, count);
        tmp_points.clear();
    }
    return std::pair<std::vector<data_type>, double>{data, max_hlf_side};
}

int main() {
    using namespace yLAB;
    auto data = give_data(std::cin); 
    admin ad{data.first.begin(), data.first.end(), data.second};
    ad.print_intersected_triangles();
}

