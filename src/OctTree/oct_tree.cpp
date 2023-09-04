#include <memory>

#include "triangle.hpp"
#include "oct_tree.hpp"
#include "utils.hpp"
#include "vector.hpp"

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
    std::vector<SubSpaces> space_coords{};
    for (size_type id = 0; id < 3; ++id) {
        space_coords.push_back(what_subspace(tria.vertices_[id]));
    }
}

BoundingCube::SubSpaces BoundingCube::what_subspace(const point_t& check_pt) const {
    vector_t point_vec {check_pt, {0, 0, 0}}; // vector from the origin

}

bool BoundingCube::is_point_inside(const point_t& pt) const {
    auto hlf_side = std::pow(SPACE_BASE, space_degree_);
    using less_eq    = std::less_equal<double>;
    using greater_eq = std::greater_equal<double>;
    return less_eq{}(std::fabs(pt.x_), fabs(center_.x_ + hlf_side)) && greater_eq{}(fabs(pt.x_), fabs(center_.x_ - hlf_side)) &&
           less_eq{}(std::fabs(pt.y_), fabs(center_.y_ + hlf_side)) && greater_eq{}(fabs(pt.y_), fabs(center_.y_ - hlf_side)) &&
           less_eq{}(std::fabs(pt.z_), fabs(center_.z_ + hlf_side)) && greater_eq{}(fabs(pt.z_), fabs(center_.z_ - hlf_side));
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

