#include "cg2/Mesh.hpp"

#include <boost/foreach.hpp>

#include <tbd/log.h>

namespace cg2
{
  void Mesh::calcBoundingBox()
  {
    boundingBox_.min.set(INF,INF,INF);
    boundingBox_.max.set(-INF,-INF,-INF);

    BOOST_FOREACH(Vertex & vertex, vertices)
    {
      Point3f v = vertex.v;
      for (int j = 0; j < 3; j++)
      {
        if (v.cell[j] > boundingBox_.max.cell[j])
        {
          boundingBox_.max[j] = v[j];
        }
        if (v.cell[j] < boundingBox_.min.cell[j])
        {
          boundingBox_.min[j] = v[j];
        }
      }
    }
  }
}
