#include "cg2/Vertex.hpp"

#include <GL/gl.h>

namespace cg2
{
  void Vertex::draw(const Color4f& _color) const
  {
    glColor4f(_color.r(),_color.g(),_color.b(),_color.a());
    glBegin(GL_POINTS);
    glVertex3fv(v.p());
    glEnd();
  }

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
