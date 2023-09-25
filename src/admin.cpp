#include <iostream>

#include "admin.hpp"

namespace yLAB {
    
void admin::print_intersected_triangles() const {
    std::set<size_type> intsec_triangles{};
    oct_tree_.find_intersecting_triangles(intsec_triangles);
    for (auto val : intsec_triangles) {
        std::cout << val << std::endl;;
    }
}

}// <--- namespace yLAB
