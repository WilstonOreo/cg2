#pragma once

#include "Primitive.hpp"

namespace cg2 
{
  /** @brief A bounding is primitive which defines bounds of a compound object
   */
  struct BoundingBox : public Primitive, public Bounds
  {
    BoundingBox() : Bounds(Point3f(INF,INF,INF),Point3f(-INF,-INF,-INF)) {}
    BoundingBox(Point3f _min, Point3f _max) : Bounds(_min,_max) {}

    bool intersect(Ray& _ray, Vec3f* _normal = NULL, Point2f* _texCoords = NULL) const;
    
    /** @brief Split bounding box in two halves 
     */
    void split(float splitPos, Axis axis, BoundingBox& boxLeft, BoundingBox& boxRight) const;

    void draw(const Color4f& _color = Color4f()) const;

    Bounds bounds() const { return Bounds(min(),max()); }

    /** @brief Return axis which largest extent
     */
    Axis dominantAxis() const;
    
    /** @brief Test if point is in box
     */
    bool pointInBox(Point3f p) const;
  };
}
