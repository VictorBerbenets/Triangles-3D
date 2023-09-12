#ifndef OCT_TREE_
#define OCT_TREE_

#include <iostream>
#include <list>
#include <set>
#include <array>
#include <cmath>
#include <memory>
#include <utility>

#include "triangle.hpp"
#include "intersector.hpp"

namespace spaceBreaking {

using namespace yLAB;

class AABB final {
    using value_type = double;
public:
    AABB(const triangle_t& tria);
    AABB(const point_t& pt1, const point_t& pt2, const point_t& pt3);
    ~AABB() = default;

    bool is_intersect(const AABB& rhs) const;

    value_type radius_x() const { return radius_[0]; };
    value_type radius_y() const { return radius_[1]; };
    value_type radius_z() const { return radius_[2]; };

    const point_t& center() const noexcept { return center_; };
private:
    point_t center_;
    std::array<value_type, 3> radius_;
}; // <--- class AABB
/*------------------------------------------------------------------------------------------------------------------------*/

class BoundingCube {
public:
    using size_type = std::size_t;
    using data_type = std::tuple<triangle_t, AABB, size_type>;
    
    static constexpr size_type VOLUMES_NUMBER = 8;

    ~BoundingCube() = default;
protected: 
    enum class SubCubes: int { A = 0, B = 1, C = 2, D = 3,
                               E = 4, F = 5, G = 6, H = 7,
                               NOT_IN_CUBE = -1 };

    using subCubes  = std::array<BoundingCube, VOLUMES_NUMBER>;
    using cubeInfo  = std::tuple<SubCubes, point_t, double>;

    static constexpr double MAX_HLF_SIDE        = std::pow(2, 32); // 2^32 - half side's length
    static constexpr size_type MAX_TREE_DEEP    = 9;

    BoundingCube();
    BoundingCube(const point_t& center, double hlf_side);

    cubeInfo what_subcube(const data_type& tria) const;
    subCubes get_subcubes(double hlf_side) const;

    void set_center() noexcept;
    bool is_point_inside(const point_t& pt) const;
//-----------------------------------------------------------------------//
    point_t center_;
    double hlf_side_;
}; // <--- class BoandingCube
/*------------------------------------------------------------------------------------------------------------------------*/

class Node final: public BoundingCube {
public:
    enum class Indicator: int {Tree_List = 1, Work_Node = 2, Tree_Root = 3};

    using pointer_type   = std::unique_ptr<Node>;
    using pointers       = std::unique_ptr<pointer_type[]>;
    using triangles_list = std::list<data_type>;
private:
    void change_id(const Indicator& id) noexcept;
    bool is_limit_reached() const noexcept;
public:
    Node(double hlf_side);
    Node(const Node& parent, double hlf_side, const Indicator& id);
    Node(const Node& parent, const point_t& center, double hlf_side, const Indicator& id, size_type deep);
    ~Node() = default;

    const pointer_type& operator[](size_type cube_sector) const;

    void insert(const data_type& tria);
    // geters
    const Node& parent() const noexcept;
    triangles_list data() const noexcept;
    Indicator get_id() const noexcept;
private:
    triangles_list inside_cube_trias_;
    pointers ptrs_childs_;
    const Node& parent_;
    size_type tree_deep_{0};
    Indicator id_;
}; // <--- class Node
/*------------------------------------------------------------------------------------------------------------------------*/

class OctTree final {
public:
    using size_type        = std::size_t;
    using value_type       = Node;
    using const_value_type = const value_type;
    using data_type        = BoundingCube::data_type;
    using collision_list   = std::list<data_type>;
private:
template<typename Collector>
    void diving_into_tree(const Node& node, Collector& col, collision_list& collision_list) const;
template<typename Collector>
    void check_collis_inside_node(Collector& col, const collision_list& ls) const;
template<typename Collector>
    void check_collis_between_nodes(Collector& col, const collision_list& ls1, const collision_list& ls2) const;
public:
    OctTree(double space_limit);
    ~OctTree() = default;

    void insert_triangle(const data_type& tria);
    const_value_type& get_root_node() const noexcept;
template<typename Collector = std::set<size_type>>
    void find_intersecting_triangles(Collector& col) const;
private:
    value_type root_node_;
}; // <--- class OctTree

template<typename Collector>
void OctTree::find_intersecting_triangles(Collector& col) const {
    collision_list root_collis {};
    diving_into_tree(root_node_, col, root_collis);
}

template<typename Collector>
void OctTree::diving_into_tree(const Node& node, Collector& col, collision_list& collision_list) const {
    static constexpr auto VOLUMES_NUMBER = BoundingCube::VOLUMES_NUMBER;

    auto node_data = node.data();
    check_collis_inside_node(col, node_data);
    check_collis_between_nodes(col, collision_list, node_data);

    if (node.get_id() == Node::Indicator::Tree_List) {
        return ;
    }
    int node_diff = node_data.size();
    collision_list.splice(collision_list.end(), node_data);      // adding collision triangles
    for (size_type count = 0; count < VOLUMES_NUMBER; ++count) {
        if (node[count]) {
            diving_into_tree(*node[count], col, collision_list);
        }
    }
    auto erase_begin = collision_list.end();
    std::advance(erase_begin, -node_diff);
    collision_list.erase(erase_begin, collision_list.end());
}

template<typename Collector>
void OctTree::check_collis_inside_node(Collector& col, const collision_list& ls) const {
    for (auto it1 = ls.cbegin(); it1 != ls.cend(); ++it1) {
        auto [tria1, aabb_1, number1] = *it1; 
        for (auto it2 = std::next(it1); it2 != ls.cend(); ++it2) {
            auto [tria2, aabb_2, number2] = *it2; 
            if (aabb_1.is_intersect(aabb_2)) {
                if (intersector::are_intersecting(tria1, tria2)) {
                    col.insert({number1, number2});
                }
            }
        }
    }
}

template<typename Collector>
void OctTree::check_collis_between_nodes(Collector& col, const collision_list& ls1, const collision_list& ls2) const {
    for (auto it1 = ls1.cbegin(); it1 != ls1.cend(); ++it1) {
        auto [tria1, aabb_1, number1] = *it1; 
        for (auto it2 = ls2.cbegin(); it2 != ls2.cend(); ++it2) {
            auto [tria2, aabb_2, number2] = *it2; 
            if (aabb_1.is_intersect(aabb_2)) {
                if (intersector::are_intersecting(tria1, tria2)) {
                    col.insert({number1, number2});
                }
            }
        }
    }
}
/*------------------------------------------------------------------------------------------------------------------------*/

} // <--- namespace spaceBreaking

#endif

