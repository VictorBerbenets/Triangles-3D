#ifndef OCT_TREE_
#define OCT_TREE_

#include <iostream>
#include <list>
#include <cmath>
#include <memory>

#include "triangle.hpp"

namespace spaceBreaking {

using namespace yLAB;

class BoundingCube {
protected:
    using size_type = std::size_t;

    static constexpr size_type MAX_SPACE_DEGREE = 64;
    static constexpr size_type SPACE_BASE       = 2;
    static constexpr double MIN_CUBE_SIDE       = 1; // = 2^space_degree

    void set_center() noexcept;
    bool is_point_inside(const point_t& pt) const;

    BoundingCube();
    BoundingCube(const point_t& center, double space_degree);
    ~BoundingCube() = default;

    bool is_tria_inside(const triangle_t& tria) const;
    double side_length() const;
//-----------------------------------------------------------------------//
    point_t center_;
    double space_degree_;
}; // <--- class BoandingCube

class Node final: protected BoundingCube {
    static constexpr std::size_t VOLUMES_NUMBER = 8;

    bool is_limit_reached() const noexcept;
    void construct_new_cubes() const;
    void set_pointers();
public:
    Node(const Node& parent);
    Node(const Node& parent, const point_t& center, double space_degree);
    ~Node() = default; 

    void insert(const triangle_t& tria);
private:
    std::list<triangle_t> inside_cube_trias_;
    std::list<triangle_t> intersec_trias_;

    std::unique_ptr<Node[]> childs_;
    const Node& parent_;

    bool are_ptrs_set_ = false;

}; // <--- class Node


class OctTree final {
    using value_type       = Node;
    using const_value_type = const value_type;
public:
    OctTree();
    ~OctTree() = default;

    void insert_triangle(const triangle_t& tria);
    const_value_type& get_root_node() const noexcept;
private:
    value_type root_node_;
    std::size_t nodes_counter_;
};

} // <--- namespace spaceBreaking

#endif

