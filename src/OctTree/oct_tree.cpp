#include <memory>
#include "triangle.hpp"
#include "oct_tree.hpp"
#include "utils.hpp"
#include "vector.hpp"

namespace spaceBreaking {

BoundingCube::BoundingCube():
                            center_{0, 0, 0},
                            hlf_side_{MAX_HLF_SIDE} {};
                            
BoundingCube::BoundingCube(const point_t& center, double new_hlf_side):
                            center_{center},
                            hlf_side_{new_hlf_side} {}              

BoundingCube::subCubes BoundingCube::get_subcubes(double hlf_side) const {
    return { BoundingCube { {center_.x_ - hlf_side, center_.y_ - hlf_side, center_.z_ + hlf_side}, hlf_side }, 
             BoundingCube { {center_.x_ - hlf_side, center_.y_ + hlf_side, center_.z_ + hlf_side}, hlf_side },
             BoundingCube { {center_.x_ - hlf_side, center_.y_ - hlf_side, center_.z_ - hlf_side}, hlf_side },
             BoundingCube { {center_.x_ - hlf_side, center_.y_ + hlf_side, center_.z_ - hlf_side}, hlf_side },
             BoundingCube { {center_.x_ + hlf_side, center_.y_ - hlf_side, center_.z_ + hlf_side}, hlf_side },
             BoundingCube { {center_.x_ + hlf_side, center_.y_ + hlf_side, center_.z_ + hlf_side}, hlf_side },
             BoundingCube { {center_.x_ + hlf_side, center_.y_ - hlf_side, center_.z_ - hlf_side}, hlf_side },
             BoundingCube { {center_.x_ + hlf_side, center_.y_ + hlf_side, center_.z_ - hlf_side}, hlf_side } };
}                                                                                                   

BoundingCube::cubeInfo BoundingCube::what_subcube(const data_type& tria) const {
    auto sb_cubes = get_subcubes(hlf_side_ / 2);
    
    auto& triangle = tria.first;
    for (size_type cubes_index = 0; cubes_index < VOLUMES_NUMBER; ++cubes_index) {
        size_type tria_index = 0;
        for ( ; tria_index < 3; ++tria_index) {
            if ( !sb_cubes[cubes_index].is_point_inside(triangle.vertices_[tria_index]) ) {
                break;
            }
        }
        if (tria_index == 3) { // all points inside the cube (triangle has 3 points)
            return { static_cast<SubCubes>(cubes_index),
                     sb_cubes[cubes_index].center_,
                     sb_cubes[cubes_index].hlf_side_ };
        }
    }
    return { SubCubes::NOT_IN_CUBE, {}, {} };
}

bool BoundingCube::is_point_inside(const point_t& pt) const {
    return  ((center_.x_ + hlf_side_) > pt.x_) && ((center_.x_ - hlf_side_) < pt.x_) &&
            ((center_.y_ + hlf_side_) > pt.y_) && ((center_.y_ - hlf_side_) < pt.y_) &&
            ((center_.z_ + hlf_side_) > pt.z_) && ((center_.z_ - hlf_side_) < pt.z_) ;
}

void Node::insert(const data_type& tria) {
    static size_type COUNT = 0;
    //std::cout << "------------------------------------PRINTING TRIANGLE-----------------------------------------\n";
   // std::cout << tria.first << std::endl;
    auto [sub_cube_t, center, hlf_side] = what_subcube(tria);
    //int id = static_cast<int>(sub_cube_t);
    //std::cout << "CUBE ID = " << id << std::endl;
    if (sub_cube_t == SubCubes::NOT_IN_CUBE) { // if the vertices of the triangle are not in any one cube
        ++COUNT;
       // std::cout << "TRIA WAS PUSHED IN NOT IN CUBE\n";
        inside_cube_trias_.push_back(tria);
       // std::cout << "-----------------------RETURN FROM NOT IN CUBE WITH COUNT = " << COUNT << "\tAND DEEP = " << tree_deep_ << "--------------------" << std::endl;
        return ;
    }
    auto cube_sector = static_cast<size_type>(sub_cube_t);
   // std::cout << "CUBE SECTOR = " << cube_sector << "\t DEEP = " << tree_deep_ << std::endl;
    if (ptrs_childs_[cube_sector] == nullptr) {
     //   std::cout << "CREATING NEW NODE WITH CUBE SECTOR = " << cube_sector << "\t AND TREE DEEP = " << tree_deep_ + 1 << std::endl;
        ptrs_childs_[cube_sector] = std::make_unique<Node>(*this, center, hlf_side, Indicator::Work_Node, tree_deep_ + 1);
        //ptrs_childs_[cube_sector]->inside_cube_trias_.push_back(tria);
    }
    if (is_limit_reached()) {
        //std::cout << "DEEP before LIMIT: " << tree_deep_ << std::endl;
       // std::cout << "LIMIT IS REACHED\n";
        ptrs_childs_[cube_sector]->change_id(Indicator::Tree_List);
        ++COUNT;
       // std::cout << "--------------------------------TRIA WAS PUSHED IN LIMIT IS REACHED-----------------------------\n";
        ptrs_childs_[cube_sector]->inside_cube_trias_.push_back(tria);
        return ;
    }
    //++tree_deep_;
    ptrs_childs_[cube_sector]->insert(tria);
    //std::cout << "COUNT = " << COUNT << std::endl;
   // std::cout << "DEEP = " << tree_deep_ << std::endl;
}

Node::Indicator Node::get_id() const noexcept {
    return id_;
}

void Node::change_id(const Indicator& id) noexcept {
    id_ = id;
}

bool Node::is_limit_reached() const noexcept {
    return tree_deep_ == 30;
    //return hlf_side_ <= MIN_CUBE_SIDE;
}

Node::Node(const Node& parent, const point_t& center, double hlf_side, const Indicator& id, size_type deep):
            BoundingCube {center, hlf_side},
            ptrs_childs_ {std::make_unique<pointer_type[]>(VOLUMES_NUMBER)},
            parent_ {parent},
            tree_deep_{deep},
            id_{id} {}
#if 0
Node::Node():
            Node{*this, {0, 0, 0}, MAX_HLF_SIDE, Indicator::Tree_Root, 0} {}

Node::Node(const Node& parent, const Indicator& id):
            Node(parent, {0, 0, 0}, MAX_HLF_SIDE, id, 0) {}
#endif

Node::Node(double hlf_side):
            Node{*this, {0, 0, 0}, hlf_side, Indicator::Tree_Root, 0} {}

Node::Node(const Node& parent, double hlf_side, const Indicator& id):
            Node(parent, {0, 0, 0}, hlf_side, id, 0) {}

OctTree::const_value_type& OctTree::get_root_node() const noexcept {
    return root_node_;
}

OctTree::OctTree(double space_limit):
                root_node_{space_limit} {}

void OctTree::insert_triangle(const data_type& tria) {
    root_node_.insert(tria);
}

const Node::pointer_type& Node::operator[](size_type cube_sector) const {
    return ptrs_childs_[cube_sector];
}

const Node& Node::parent() const noexcept {
    return parent_;
}

Node::triangles_list Node::data() const noexcept {
    return inside_cube_trias_;
}

} // <--- namespace spaceBreaking

