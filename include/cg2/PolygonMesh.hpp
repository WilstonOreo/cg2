#pragma once

#include "cg2/Mesh.hpp"

namespace cg2
{
  struct PolygonMesh : public Mesh
  {
      void read(string filename);

      void draw(Color color = Color());
      void displayNormals();

      TBD_DECLARE_PROPERTY_REF(Polygons,polygons);
    protected:
      void calcNormals();
  };
}
