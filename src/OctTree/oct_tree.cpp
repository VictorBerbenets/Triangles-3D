
#include "triangle.hpp"
#include "oct_tree.hpp"

namespace spaceBreaking {

BoundingCube::BoundingCube():
                            center_{0, 0, 0},
                            side_size_{MAX_CUBE_SIDE} {}
                            
BoundingCube::BoundingCube(const point_t& center, double side_size):
                            center_{center},
                            side_size_{side_size} {}

void Node::insert(const triangle_t& tria) const {
    
}

OctTree::const_value_type& OctTree::get_root_node() const noexcept {
    return root_node_;
}

void OctTree::insert_triangle(const triangle_t& tria) {
    root_node_.insert(tria);
    ++nodes_counter_;
}

} // <--- namespace spaceBreaking
