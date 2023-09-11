#include <iostream>

#include <cmath>
#include <chrono>

#include "admin.hpp"

decltype(auto) give_data(std::istream& stream) {
    using namespace yLAB;
    using dataVal   = admin::dataVal;
    using size_type = admin::size_type;

    size_type data_size{};
    stream >> data_size;
    if (!stream.good()) {
        throw std::runtime_error{"data size reading error\n"};
    }

    double max_hlf_side = 0;

    std::vector<dataVal> data{};
    data.reserve(data_size);

    std::vector<double> tmp_points{};
    tmp_points.reserve(admin::SET_POINTS_SIZE); //  processing of input data

    for (size_type count = 1; count <= data_size; ++count) {
        double tmp_value{};
        for (size_type points_number = 0; points_number < admin::SET_POINTS_SIZE; ++points_number) {
            stream >> tmp_value;
            if (!stream.good()) {
                throw std::runtime_error{"data reading error\n"};
            }
            if (std::greater<double>{}(std::fabs(tmp_value), max_hlf_side)) {
                max_hlf_side = std::fabs(tmp_value);
            }
            tmp_points.push_back(tmp_value);
        }
        auto cell_data = dataVal( triangle_t { {tmp_points[0], tmp_points[1], tmp_points[2]},
                                               {tmp_points[3], tmp_points[4], tmp_points[5]},
                                               {tmp_points[6], tmp_points[7], tmp_points[8]} }, count );
        data.push_back(cell_data);
        tmp_points.clear();
    }
    return std::pair<std::vector<dataVal>, double>{data, max_hlf_side};
}

int main() {
    using namespace yLAB;

    auto start = std::chrono::high_resolution_clock::now(); 
    
    admin ad{give_data(std::cin)};
    ad.print_intersected_triangles();
   
    auto end = std::chrono::high_resolution_clock::now(); 
    std::chrono::duration<double> dur = end - start;
    std::cout << "TIME: " << dur.count() << std::endl;
}

