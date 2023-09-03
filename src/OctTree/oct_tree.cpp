
#include "triangle.hpp"
#include "oct_tree.hpp"

namespace spaceBreaking {

OctTree::OctTree(): root_node_.parent_(nullptr) {}

OctTree::const_value_type OctTree::get_root_node() const noexcept {
    return root_node_;
}

void OctTree::insert_triangle(const triangle_t& tria) const {
    root_node_.insert(tria);
}

} // <--- namespace spaceBreaking
