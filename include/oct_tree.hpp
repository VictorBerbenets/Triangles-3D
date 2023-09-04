#ifndef OCT_TREE_
#define OCT_TREE_

#include <iostream>
#include <list>
#include <array>
#include <cmath>
#include <memory>
#include <utility>

#include "triangle.hpp"

namespace spaceBreaking {

using namespace yLAB;

class BoundingCube {
public:
    using size_type = std::size_t;
    using data_type    = std::pair<triangle_t, size_type>;
    
    ~BoundingCube() = default;
protected:
    static constexpr size_type VOLUMES_NUMBER = 8;
    
    enum class SubCubes: int { A = 0, B = 1, C = 2, D = 3,
                               E = 4, F = 5, G = 6, H = 7,
                               NOT_IN_CUBE = -1 };

    using subCubes  = std::array<BoundingCube, VOLUMES_NUMBER>;
    using cubeInfo  = std::tuple<SubCubes, point_t, size_type>;

    static constexpr size_type MAX_SPACE_DEGREE = 32; // 2^32 - half side's length
    static constexpr size_type DEGREE_DECREASE  = 1;  //  divide each volume by 2
    static constexpr size_type SPACE_BASE       = 2;  //
    static constexpr double MIN_CUBE_SIDE       = 1;  // = 2^space_degree
 
    BoundingCube();
    BoundingCube(const point_t& center, size_type space_degree);

    cubeInfo what_subcube(const data_type& tria) const;
    subCubes get_subcubes(double hlf_side, size_type new_degree) const;

    void set_center() noexcept;
    bool is_point_inside(const point_t& pt) const;

    double side_length(size_type space_degree) const;
//-----------------------------------------------------------------------//
    point_t center_;
    size_type space_degree_;
}; // <--- class BoandingCube

class Node final: protected BoundingCube {
    using pointer_type = std::unique_ptr<Node>;

    bool is_limit_reached() const noexcept;
public:
    Node(const Node& parent);
    Node(const Node& parent, const point_t& center, size_type space_degree);
    ~Node() = default;

    void insert(const data_type& tria);
private:
    std::list<data_type> inside_cube_trias_;
    std::list<data_type> intersec_trias_;

    std::unique_ptr<pointer_type[]> ptrs_childs_;
    const Node& parent_;

    bool are_ptrs_set_ = false;

}; // <--- class Node


class OctTree final {
    using value_type       = Node;
    using const_value_type = const value_type;
    using data_type        = BoundingCube::data_type;
public:
    OctTree();
    ~OctTree() = default;

    void insert_triangle(const data_type& tria);
    const_value_type& get_root_node() const noexcept;
private:
    value_type root_node_;
    std::size_t nodes_counter_;
}; // <--- class OctTree


} // <--- namespace spaceBreaking

#endif

