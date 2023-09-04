#include <memory>

#include "triangle.hpp"
#include "oct_tree.hpp"
#include "utils.hpp"

namespace spaceBreaking {


BoundingCube::BoundingCube():
                            center_{0, 0, 0},
                            space_degree_{MAX_SPACE_DEGREE} {}
                            
BoundingCube::BoundingCube(const point_t& center, double space_degree):
                            center_{center},
                            space_degree_{space_degree} {}


double BoundingCube::side_length() const {
    return std::pow(2, space_degree_);
}

bool BoundingCube::is_tria_inside(const triangle_t& tria) const {
    
}

bool BoundingCube::is_point_inside(const point_t& pt) const {
    auto hlf_side = std::pow(SPACE_BASE, space_degree_);
    return  fabs(pt.x_) <= fabs(center_.x_ + hlf_side) && fabs(pt.x_) >= fabs(center_.x_ - hlf_side) &&
            fabs(pt.y_) <= fabs(center_.y_ + hlf_side) && fabs(pt.y_) >= fabs(center_.y_ - hlf_side) &&
            fabs(pt.z_) <= fabs(center_.z_ + hlf_side) && fabs(pt.z_) >= fabs(center_.z_ - hlf_side);
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
        
        if (is_tria_inside(tria)) {
            
        }
    }
}

Node::Node(const Node& parent, const point_t& center, double space_degree):
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

