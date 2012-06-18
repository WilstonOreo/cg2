#include "cg2/Vertex.hpp"

namespace cg2
{
  Vec3f Polygon::normal()
  {
    Vec3f n;
    int count = 0;
    for (unsigned i = 0; i < size()-2; i++)
    {
      Point3f const & a = vertices.at(i  )->v;
      Point3f const & b = vertices.at(i+2)->v;
      Point3f const & c = vertices.at(i+1)->v;
      n += (a-b).cross(c-b);
      count++;
    }
    if (count > 0)
    {
      n.normalize();
    }
    return n;
  }
}
