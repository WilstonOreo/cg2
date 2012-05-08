#pragma once

#include "cg2/misc.hpp"
#include "cg2/Vector.hpp"
#include <vector>

namespace cg2 
{
  struct Object
  {
    virtual void draw(Color color = Color()) = 0;
  };

  typedef std::vector<Object*> ObjectList;
  typedef std::vector<Object> Objects;
}
