#ifndef OCT_TREE_
#define OCT_TREE_

#include <iostream>
#include <functional>
#include <unordered_set>
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
    using data_type = std::pair<triangle_t, size_type>;
    
    static constexpr size_type VOLUMES_NUMBER = 8;

    ~BoundingCube() = default;
protected: 
    enum class SubCubes: int { A = 0, B = 1, C = 2, D = 3,
                               E = 4, F = 5, G = 6, H = 7,
                               NOT_IN_CUBE = -1 };

    using subCubes  = std::array<BoundingCube, VOLUMES_NUMBER>;
    using cubeInfo  = std::tuple<SubCubes, point_t, size_type>;

    static constexpr size_type MAX_SPACE_DEGREE = 32; // 2^32 - half side's length
    static constexpr size_type DEGREE_DECREASE  = 1;  //  divide each volume by 2
    static constexpr size_type SPACE_BASE       = 2;  //
    static constexpr double MIN_CUBE_SIDE       = 5;  // = 2^space_degree
 
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

class Node final: public BoundingCube {
    using pointer_type   = std::unique_ptr<Node>;
    using pointers       = std::unique_ptr<pointer_type[]>;
    using triangles_list = std::list<data_type>;

    enum class Indicator: int {Tree_List = 1, Work_Node = 2, Tree_Root = 3};

    void change_id(const Indicator& id) noexcept;
    bool is_limit_reached() const noexcept;
public:
    Node();
    Node(const Node& parent, const Indicator& id);
    Node(const Node& parent, const point_t& center, size_type space_degree, const Indicator& id);
    ~Node() = default;

    const pointer_type& operator[](const SubCubes& cube_sector) const;

    void insert(const data_type& tria);
    // geters
    const Node& parent() const noexcept;
    const triangles_list& data() const noexcept;
//    decltype(auto) pointer(const SubCubes& cube_sector = SubCubes::NOT_IN_CUBE) const;
//    const pointers& childs() const;
private:
    triangles_list inside_cube_trias_;

    pointers ptrs_childs_;
    const Node& parent_;

    Indicator id_;

}; // <--- class Node


class OctTree {
public:
    using size_type        = std::size_t;
    using value_type       = Node;
    using const_value_type = const value_type;
    using data_type        = BoundingCube::data_type;
    using collision_list   = std::list<data_type>; 
    using trias_compare    = std::function<bool(const triangle_t&, const triangle_t&)>;
private:
template<typename Collector = std::unordered_set<size_type>> 
    void diving_into_tree(Collector& col, collision_list& collision_list) const; 
public:
    OctTree() {};
    ~OctTree() = default;
    
    void insert_triangle(const data_type& tria);
    const_value_type& get_root_node() const noexcept;
template<typename Collector = std::unordered_set<size_type>> 
    void find_intersecting_triangles(Collector& col, const trias_compare& comporator = trias_compare()) const;
private:
    value_type root_node_;
    std::size_t nodes_counter_;
}; // <--- class OctTree

template<typename Collector>
void OctTree::find_intersecting_triangles(Collector& col, const trias_compare& comporator) const {    
    std::list<data_type> collision_list;
    auto root_collis= root_node_.data();
    for (auto iter1 = root_collis.begin(); iter1 != root_collis.end(); ++iter1) {
        for (auto iter2 = std::next(iter1); iter2 != root_collis.end(); ++iter2) {
            if (comporator(iter1->first, iter2->first)) {
                col.insert({iter1->second, iter2->second});
            }
        }
    }   
    diving_into_tree(col, collision_list);
}

template<typename Collector>
void OctTree::diving_into_tree(Collector& col, collision_list& collision_list) const {
    auto VOLUMES_NUMBER = BoundingCube::VOLUMES_NUMBER;
    
    for (size_type count = 0; count < VOLUMES_NUMBER; ++count) {
        
    }
}

} // <--- namespace spaceBreaking

#endif

