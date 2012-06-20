#pragma once

#include "cg2/misc.hpp"
#include "cg2/Color.hpp"
#include <vector>

namespace cg2
{
  struct Object
  {
    virtual void draw(const Color4f& _color = Color4f()) const = 0;
  };

  typedef std::vector<Object *> ObjectList;
  typedef std::vector<Object> Objects;
}
