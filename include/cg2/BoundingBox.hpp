#pragma once

#include "Primitive.hpp"

namespace cg2 
{
  /** @brief A bounding is primitive which defines bounds of a compound object
   */
  struct BoundingBox : public Primitive, public Bounds
  {
    BoundingBox() : Bounds() {}
    BoundingBox(Point3f _min, Point3f _max) : Bounds(_min,_max) {}

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
