#include <iostream>
#include <vector>
#include <set>
#include <functional>
#include <stdexcept>
#include <chrono>

#include "admin.hpp"
#include "intersector.hpp"
#include "oct_tree.hpp"
#include "plane.hpp"
#include "triangle.hpp"

namespace yLAB {


bool intersector::are_intersecting(const triangle_t& tria1, const triangle_t& tria2) {
        tria_plane pair1 = {tria1, tria1.get_plane()};
        tria_plane pair2 = {tria2, tria2.get_plane()};
        if (!pair1.second.is_parallel(pair2.second)) {
            if (different_intersection(pair1, pair2)) {
                return true;
            }
        } else if (pair1.second == pair2.second) {    // both triangles lies in one plane
            if (same_intersection(pair1.first, pair2.first)) {
                return true;
            }
        }
        return false;
}

void admin::print_intersected_triangles() const {
    std::set<size_type> intsec_triangles{};
    oct_tree_.find_intersecting_triangles(intsec_triangles);
    for (auto val : intsec_triangles) {
        std::cout << val << ' ';
    }
    std::cout << std::endl;
}

} // <--- namespace yLAB

