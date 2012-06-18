#pragma once

#include "cg2/Vertex.hpp"

namespace cg2
{
  class MeshReader
  {
  public:
    virtual void read(std::string filename, vector<Vertex> * vertices = NULL, vector<Polygon> * polygons = NULL) = 0;
  };
}
