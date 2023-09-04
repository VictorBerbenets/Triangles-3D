#include <memory>

#include "triangle.hpp"
#include "oct_tree.hpp"
#include "utils.hpp"
#include "vector.hpp"

namespace spaceBreaking {


BoundingCube::BoundingCube():
                            center_{0, 0, 0},
                            space_degree_{MAX_SPACE_DEGREE} {}
                            
BoundingCube::BoundingCube(const point_t& center, size_type space_degree):
                            center_{center},
                            space_degree_{space_degree} {}


double BoundingCube::side_length(size_type space_degree) const {
    return std::pow(2, space_degree);
}


BoundingCube::subcubes BoundingCube::get_subcubes(double hlf_side, size_type new_degree) const {
    return { BoundingCube { {center_.x_ - hlf_side, center_.y_ - hlf_side, center_.z_ + hlf_side}, new_degree }, 
             BoundingCube { {center_.x_ - hlf_side, center_.y_ + hlf_side, center_.z_ + hlf_side}, new_degree },
             BoundingCube { {center_.x_ - hlf_side, center_.y_ - hlf_side, center_.z_ - hlf_side}, new_degree },
             BoundingCube { {center_.x_ - hlf_side, center_.y_ + hlf_side, center_.z_ - hlf_side}, new_degree },
             BoundingCube { {center_.x_ + hlf_side, center_.y_ - hlf_side, center_.z_ + hlf_side}, new_degree },
             BoundingCube { {center_.x_ + hlf_side, center_.y_ + hlf_side, center_.z_ + hlf_side}, new_degree },
             BoundingCube { {center_.x_ + hlf_side, center_.y_ - hlf_side, center_.z_ - hlf_side}, new_degree },
             BoundingCube { {center_.x_ + hlf_side, center_.y_ + hlf_side, center_.z_ - hlf_side}, new_degree } };
}

BoundingCube::SubCubes BoundingCube::what_subspace(const triangle_t& tria) const {
    auto new_degree = space_degree_ - DEGREE_DECREASE;
    auto sb_cubes = get_subcubes(side_length(new_degree), new_degree);

    for (size_type cubes_index = 0; cubes_index < CUBES_SPACE_NUMBER; ++cubes_index) {
        size_type tria_index = 0;
        for ( ; tria_index < 3; ++tria_index) {
            if ( !sb_cubes[cubes_index].is_point_inside(tria.vertices_[tria_index]) ) {
                break;
            }
        }
        if (tria_index == 3) {
            return static_cast<SubCubes>(cubes_index);
        }
    }
    return SubCubes::NOT_IN_CUBE;
}

bool BoundingCube::is_point_inside(const point_t& pt) const {
    using less    = std::less<double>;
    using greater = std::greater<double>;

    auto hlf_side = side_length(space_degree_);
    return less{}(std::fabs(pt.x_), fabs(center_.x_ + hlf_side)) && greater{}(fabs(pt.x_), fabs(center_.x_ - hlf_side)) &&
           less{}(std::fabs(pt.y_), fabs(center_.y_ + hlf_side)) && greater{}(fabs(pt.y_), fabs(center_.y_ - hlf_side)) &&
           less{}(std::fabs(pt.z_), fabs(center_.z_ + hlf_side)) && greater{}(fabs(pt.z_), fabs(center_.z_ - hlf_side));
}


void Node::set_pointers() {
    
}

void Node::insert(const triangle_t& tria) {
    if (is_zero(space_degree_)) {
        return ;
    }

    if (!are_ptrs_set_) {
        set_pointers();
        are_ptrs_set_ = true;
    }

    for (size_type count = 0; count < VOLUMES_NUMBER; ++count) {
        
    }
}

Node::Node(const Node& parent, const point_t& center, size_type space_degree):
            BoundingCube {center, space_degree},
            ptrs_childs_ {std::make_unique<pointer_type[]>(VOLUMES_NUMBER)},
            parent_ {parent} {}

OctTree::const_value_type& OctTree::get_root_node() const noexcept {
    return root_node_;
}

void OctTree::insert_triangle(const triangle_t& tria) {
    root_node_.insert(tria);
    ++nodes_counter_;
}

} // <--- namespace spaceBreaking

