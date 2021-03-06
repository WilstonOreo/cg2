#pragma once

#include "cg2/MeshReader.hpp"

namespace cg2 
{
  class OFFReader
  {
    public:
      void read(std::string filename, vector<Vertex>* vertices = NULL, vector<Triangle>* triangles = NULL);
      void read(std::string filename, vector<Vertex>* vertices, vector<VertexTriangle>* triangles);
  };
}
