#pragma once

#include "cg2/Primitive.hpp"

namespace cg2 
{
  struct Plane : public Primitive 
  {
    Plane(const Point3f& _center, const Vec3f& _normal)
      : center_(_center), normal_(_normal)
    {}

    bool intersect(Ray& _ray, Vec3f* _normal = NULL, Point2f* _texCoords = NULL) const;

    void draw(Color4f const & color = Color4f()) const {}

    Bounds bounds() const;

    Point3f center_;
    Vec3f normal_;
  };
}
