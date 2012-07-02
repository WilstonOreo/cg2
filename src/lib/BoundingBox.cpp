#include "cg2/BoundingBox.hpp"

#include <GL/gl.h>

using namespace std;

namespace cg2 
{
  bool BoundingBox::pointInBox(const Point3f p) const
  {
    return p.x() > min().x() && p.x() < max().x() &&
      p.y() > min().y() && p.y() < max().y() &&
      p.z() > min().z() && p.z() < max().z();
  }

  Axis BoundingBox::dominantAxis() const
  {
    Vec3f d = size(); 
    if (d.x() > d.y())
    { 	if (d.x() > d.z()) return X;
    } else
      if (d.y() > d.z()) return Y;
    return Z;
  }

  void BoundingBox::split(float splitPos, Axis axis, BoundingBox& boxLeft, BoundingBox& boxRight) const
  {
    boxLeft(min(),max());
    boxRight(min(),max());
    boxLeft.max()[axis] = splitPos;
    boxRight.min()[axis] = splitPos;
  }

  void BoundingBox::draw(const Color4f& _color) const
  {
    float x  = min().x(), y  = min().y(), z  = min().z();
    float xs = max().x(), ys = max().y(), zs = max().z();

    glColor4f(_color.r(),_color.g(),_color.b(),_color.a());

    glBegin(GL_LINE_LOOP);
    // top side
    glVertex3f(x , ys, z );
    glVertex3f(xs, ys, z );
    glVertex3f(xs, ys, zs);
    glVertex3f(x , ys, zs);
    glEnd();

    glBegin(GL_LINE_LOOP);
    // bottom side
    glVertex3f(x , y, z );
    glVertex3f(xs, y, z );
    glVertex3f(xs, y, zs);
    glVertex3f(x , y, zs);
    glEnd();

    glBegin(GL_LINE_LOOP);
    // east side
    glVertex3f(x , y , z);
    glVertex3f(xs, y , z);
    glVertex3f(xs, ys, z);
    glVertex3f(x , ys, z);
    glEnd();

    glBegin(GL_LINE_LOOP);
    // west side
    glVertex3f(x , y ,zs);
    glVertex3f(xs, y ,zs);
    glVertex3f(xs, ys,zs);
    glVertex3f(x , ys,zs);
    glEnd();

    glBegin(GL_LINE_LOOP);
    // north side
    glVertex3f(x     , y     , z     );
    glVertex3f(x     , y     , zs);
    glVertex3f(x     , ys, zs);
    glVertex3f(x     , ys, z     );
    glEnd();

    glBegin(GL_LINE_LOOP);
    // south side
    glVertex3f( xs, y     , z     );
    glVertex3f( xs, y     , zs);
    glVertex3f( xs, ys, zs);
    glVertex3f( xs, ys, z     );
    glEnd();    
  }
}
