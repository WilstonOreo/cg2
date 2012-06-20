#pragma once

#include "cg2/Ray.hpp"
#include "cg2/Object.hpp"
#include "cg2/Bounds.hpp"

namespace cg2 
{
  /* @brief A primitive is an object which has an extent and for which an intersection point can be found 
  */
  struct Primitive : public Object
  {
    /** @brief Virtual method to determine intersection point
     * @param _ray        Ray for intersection
     * @param _normal     Pointer to normal determined from intersection
     * @param _texCoords  Pointer to texCoords to be returned
     */
    virtual bool intersect(Ray& _ray, Vec3f* _normal = NULL, Point2f* _texCoords = NULL) const = 0;

    /** @brief Return bounds of primitive
     */
    virtual Bounds bounds() const = 0;

    /** @brief Return pointer to object
     */
    Primitive* pointer() const { return const_cast<Primitive*>(this); }
  };
}
