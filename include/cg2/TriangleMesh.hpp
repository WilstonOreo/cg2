#pragma once

#include "cg2/Triangle.hpp"

namespace cg2 
{
  struct TriangleMesh : public Mesh, public ShadableObject
  {
    void readOff(string filename);

    TBD_DECLARE_PROPERTY_REF(Triangles,triangles);
    protected:
    void calcTriangles();
  };
}
