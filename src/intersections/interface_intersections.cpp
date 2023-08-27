#include "intersector.hpp"

namespace yLAB {

intersector::intersector(std::istream& is):
    stream_{is} {
    size_type data_size{};
    stream_ >> data_size;
    if (!stream_.good()) {
        throw std::runtime_error{"data size reading error\n"};
    }
    data_.reserve(data_size);

    std::vector<double> tmp_points{};
    tmp_points.reserve(SET_POINTS_SIZE);
    //  processing of input data 
    for (size_type count = 1; count <= data_size; ++count) {
        double tmp_value{};
        for (size_type points_number = 0; points_number < SET_POINTS_SIZE; ++points_number) {
            stream_ >> tmp_value;
            if (!stream_.good()) {
                throw std::runtime_error{"data reading error\n"};
            }
            tmp_points.push_back(tmp_value);
        }
        data_.emplace_back(triangle_t { {tmp_points[0], tmp_points[1], tmp_points[2]},
                                        {tmp_points[3], tmp_points[4], tmp_points[5]},
                                        {tmp_points[6], tmp_points[7], tmp_points[8]} }, count);
        tmp_points.clear();
    };
}

void intersector::print_intersected_triangles() const {
    std::unordered_set<size_type> intersec_triangles{};
    for (auto iter1 = data_.begin(); iter1 != data_.end(); ++iter1) {
        auto comp_plane = iter1->first.get_plane();
        for (auto iter2 = (iter1 + 1); iter2 != data_.end(); ++iter2) {
            auto tmp_plane = iter2->first.get_plane();
            if (comp_plane == tmp_plane) { // both triangles lies in one plane

            } else if (comp_plane.is_parallel(tmp_plane).first) {
                // these pair of triangles can't intersect each other
                continue;
            } else { // both triangles lies in different planes
               auto pair = different_intersection(iter1->first, iter2->first); 
            }
        }
    }
}

};

