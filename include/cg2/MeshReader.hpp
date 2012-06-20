#pragma once

#include "cg2/Vertex.hpp"
#include "cg2/Triangle.hpp"

namespace cg2 
{
  class MeshReader 
  {
  public:
    virtual void read(std::string filename, 
                      vector<Vertex>* vertices = NULL, 
                      vector<Triangle>* triangles = NULL) = 0;
  };
}
