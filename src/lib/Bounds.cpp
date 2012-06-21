#include "cg2/Bounds.hpp"

namespace cg2 
{
    void Bounds::operator()(const Point3f& _min, const Point3f& _max)
    {
      min_ = _min; 
      max_ = _max; 
      validate();
    }

    std::vector<Point3f> Bounds::corners() const
    {
      std::vector<Point3f> _corners(8);
      _corners[0] = min_;
      _corners[1] = Point3f(min_.x(),min_.y(),max_.z());
      _corners[2] = Point3f(min_.x(),max_.y(),min_.z());
      _corners[3] = Point3f(min_.x(),max_.y(),max_.z());
      _corners[4] = Point3f(min_.x(),min_.y(),min_.z());
      _corners[5] = Point3f(max_.x(),min_.y(),max_.z());
      _corners[6] = Point3f(max_.x(),max_.y(),min_.z());
      _corners[7] = max_;
      return _corners;
    }
    
    void Bounds::validate()
    {
      FOREACH_AXIS 
        if (min_[axis] > max_[axis]) 
          std::swap(min_[axis],max_[axis]);
    }

    void Bounds::extend(const Bounds& _bounds)
    {
      FOREACH_AXIS
      {
        min_[axis] = std::min(_bounds.min_[axis],min_[axis]);
        max_[axis] = std::min(_bounds.max_[axis],max_[axis]);
      }
      validate();
    }

    Vec3f Bounds::size() const { return max_ - min_; }

}
