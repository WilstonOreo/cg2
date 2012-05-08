#pragma once

#include "cg2/Vertex.hpp"

namespace cg2
{
  class PointCloud : public Mesh
  {
    void read(string filename);
    void buildKDTree();
    void draw(color3f Color = color3f());

    void divideNode(KDNode* node, BoundingBox& box, int depth);
  };
}
