#pragma once

#include "cg2/Vector.hpp"

#include <vector>

namespace cg2
{
  struct Light
  {
    TBD_PROPERTY_REF(Point3f,pos);
    TBD_PROPERTY_REF(Color,ambient);
    TBD_PROPERTY_REF(Color,diffuse);
    TBD_PROPERTY_REF(Color,specular);
    TBD_PROPERTY(float,intensity);
    TBD_PROPERTY(float,shadows);
    TBD_PROPERTY(float,radius);
  };

  typedef std::vector<Light *> LightList;
  typedef std::vector<Light> Lights;
}

