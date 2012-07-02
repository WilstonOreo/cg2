#pragma once

#include "cg2/Color.hpp"
#include "cg2/Bounds.hpp"

namespace cg2 
{
  /* @brief A primitive is an object which has an extent and for which an intersection point can be found 
  */
  struct Primitive 
  {
    virtual void draw(const Color4f& _color = Color4f()) const = 0;

    /** @brief Return bounds of primitive
     */
    virtual Bounds bounds() const = 0;

    /** @brief Return pointer to object
     */
    Primitive* pointer() const { return const_cast<Primitive*>(this); }
  };
}
