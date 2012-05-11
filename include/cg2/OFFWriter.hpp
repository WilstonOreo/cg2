#pragma once

#include "cg2/MeshWriter.hpp"

namespace cg2 
{
  class OFFWriter
  {
    public:
      void write(std::string filename, Vertices* vertices = NULL, Polygons* polygons = NULL);
  };
}
