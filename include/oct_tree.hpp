#ifndef OCT_TREE_
#define OCT_TREE_

#include <iostream>
#include <list>
#include <array>
#include <cmath>
#include <memory>

#include "triangle.hpp"

namespace spaceBreaking {

using namespace yLAB;

class BoundingCube {
public:
    ~BoundingCube() = default;
protected:
    using size_type = std::size_t;

    static constexpr size_type CUBES_SPACE_NUMBER = 8;

    using subcubes  = std::array<BoundingCube, CUBES_SPACE_NUMBER>;

    enum class SubCubes: char { A = 0, B = 1, C = 2, D = 3,
                                E = 4, F = 5, G = 6, H = 7,
                                NOT_IN_CUBE = -1 };

    static constexpr size_type MAX_SPACE_DEGREE = 32; // 2^32 - half side's length
    static constexpr size_type DEGREE_DECREASE  = 1;  //  divide each volume by 2
    static constexpr size_type SPACE_BASE       = 2;  // 
    static constexpr double MIN_CUBE_SIDE       = 1;  // = 2^space_degree

    BoundingCube();
    BoundingCube(const point_t& center, size_type space_degree);
    
    SubCubes what_subspace(const triangle_t& tria) const;
    subcubes get_subcubes(double hlf_side, size_type new_degree) const;

    void set_center() noexcept;
    bool is_point_inside(const point_t& pt) const;
    
    double side_length(size_type space_degree) const;
//-----------------------------------------------------------------------//
    point_t center_;
    size_type space_degree_;
}; // <--- class BoandingCube

class Node final: protected BoundingCube {
    using pointer_type = std::unique_ptr<Node>;
    
    static constexpr std::size_t VOLUMES_NUMBER = 8;
    
    bool is_limit_reached() const noexcept;
    void construct_new_cubes() const;
    void set_pointers();
public:
    Node(const Node& parent);
    Node(const Node& parent, const point_t& center, size_type space_degree);
    ~Node() = default; 

    void insert(const triangle_t& tria);
private:
    std::list<triangle_t> inside_cube_trias_;
    std::list<triangle_t> intersec_trias_;

    std::unique_ptr<pointer_type[]> ptrs_childs_;
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

