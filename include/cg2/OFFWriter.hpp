#pragma once

#include "cg2/MeshWriter.hpp"

namespace cg2 
{
  class OFFWriter
  {
    public:
      void write(std::string filename, const Vertices* vertices = NULL, const Polygons* polygons = NULL) const;
  };
}
