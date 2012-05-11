#include "cg2/Vertex.hpp"

namespace cg2
{
  Vec3f Polygon::normal()
  {
  	Vec3f n; int count = 0;
	  for (unsigned i = 0; i < size()-2; i++)
	  {
		  Point3f a = at(i)->v,b = at(i+2)->v,c = at(i+1)->v;
		  n += (a-b).cross(c-b);		
		  count++;
	  }
	  if (count > 0) n.normalize();
	  return n;
  }
}
