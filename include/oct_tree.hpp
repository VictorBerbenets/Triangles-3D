#include <iostream>
#include <list>
#include <memory>

#include "triangle.hpp"

namespace spaceBreaking {

using namespace yLAB;

class BoandingCube {

public:

protected:

private:

}; // <--- class BoandingCube

class OctTree final: protected BoandingCube {

public:


private:
    std::list<triangle_t> data_;
    
    std::unique_ptr<OctTree> left_;
    std::unique_ptr<OctTree> right_;

    std::unique_ptr<OctTree> parent_;
}; // <--- class OctTree

} // <--- namespace spaceBreaking
