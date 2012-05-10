#pragma once

#include "cg2/Vertex.hpp"

namespace cg2 
{
  class MeshReader 
  {
  public:
    virtual void read(std::string filename, Vertices* vertices = NULL, Polygons* polygons = NULL) = 0;
  };
}
