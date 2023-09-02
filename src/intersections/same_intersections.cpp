
#include "intersector.hpp"
#include "utils.hpp"
#include "point.hpp"
#include "triangle.hpp"

namespace yLAB {

bool intersector::same_intersection(const triangle_t& tria1, const triangle_t& tria2) const {
    static constexpr std::size_t POINTS_TO_COMPARE = 3;
    for (std::size_t index1 = 0; index1 < POINTS_TO_COMPARE; ++index1) {
        std::size_t less_zero{0};
        for (std::size_t index2 = 0; index2 < POINTS_TO_COMPARE; ++index2) {
            if (!mul_vect_products({ tria1.vertices_[index2],
                                     tria1.vertices_[(index2 + 1) % POINTS_TO_COMPARE],
                                     tria1.vertices_[(index2 + 2) % POINTS_TO_COMPARE] },
                                     tria2.vertices_[index1]) ) { break; }
        }
        return true;
    }
    return false;
}

bool intersector::mul_vect_products(const triangle_t::pts_vector& tria, const point_t& comp_pt) const {
    vector_t dir_tria1  {tria[0], tria[1]};
    vector_t dir_tria2  {tria[0], tria[2]};
    vector_t points_dir {tria[0], comp_pt};

    return scalar_multiply (calc_vects_product(points_dir, dir_tria1),
                            calc_vects_product(points_dir, dir_tria2)) <= 0;
}

}

