#ifndef VECTOR_
#define VECTOR_

#include <array>

#include "point.hpp"

namespace yLAB {

class vector_t final {
    using size_type      = std::size_t;
    using iterator       = typename std::array<double, 3>::iterator;
    using const_iterator = const iterator;
    using value_type     = double;
public:
    vector_t(double coord1, double coord2, double coord3);
    vector_t(const point_t& pt1, const point_t& pt2);
    vector_t(double coord_for_all);
    vector_t();
    ~vector_t() = default;
    
    vector_t operator*(double coeff) const noexcept;
    vector_t operator/(double coeff) const;
    
    double& operator[](size_type index) noexcept;
    const double& operator[](size_type index) const noexcept;
    double& operator()(size_type index);
    const double&  operator()(size_type index) const;

    double get_module() const;
    
    bool is_null() const;
    bool is_valid() const;
private:
    std::array<double, 3> coords_;
};


} // <--- namespace yLAB

#endif
