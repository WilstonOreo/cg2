#include "cg2/Triangle.hpp"

#include <GL/gl.h>

namespace cg2
{
  void Triangle::draw(const Color4f& _color) const
  {
    glColor4f(_color.r(),_color.g(),_color.b(),_color.a());
    glBegin(GL_TRIANGLES);
      glNormal3f(COORDS(n_));
      glVertex3f(COORDS(v0()));
      glVertex3f(COORDS(v1()));
      glVertex3f(COORDS(v2()));
    glEnd();
  }

  void VertexTriangle::draw(const Color4f& _color) const
  {
    glColor4f(_color.r(),_color.g(),_color.b(),_color.a());
    glBegin(GL_TRIANGLES);
      glNormal3f(COORDS(v[0]->n));
      glVertex3f(COORDS(v[0]->v));
      glNormal3f(COORDS(v[1]->n));
      glVertex3f(COORDS(v[1]->v));
      glNormal3f(COORDS(v[2]->n));
      glVertex3f(COORDS(v[2]->v));
    glEnd();

  }
}
