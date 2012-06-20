#include "cg2/Triangle.hpp"

#include <GL/gl.h>

namespace cg2
{
  Triangle::draw(const Color4f& _color) const
  {
    glColor4f(_color.r(),_color.g(),_color.b(),_color.a());
    glBegin(GL_TRIANGLES);
      glNormal3fv(n_.p());
      glVertex3fv(v0().p());
      glVertex3fv(v1().p());
      glVertex3fv(v2().p());
    glEnd();
  }

  VertexTriangle::draw(const Color4f& _color) const
  {
    glColor4f(_color.r(),_color.g(),_color.b(),_color.a());
    glBegin(GL_TRIANGLES);
      glNormal3fv(v[0]->n.p());
      glVertex3fv(v[0]->v.p());
      glNormal3fv(v[1]->n.p());
      glVertex3fv(v[1]->v.p());
      glNormal3fv(v[2]->n.p());
      glVertex3fv(v[2]->v.p());
    glEnd();

  }
}
