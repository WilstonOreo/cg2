#include "cg2/Plane.hpp"

#include <GL/glut.h>

namespace cg2 
{
  bool Plane::intersect(Ray& ray)
  {
    float dn = ray.dir * _n;
    if (dn == 0.0f) return false;

    Vec3f org = ray.org - _c;
    float d = -(_n * org) / dn;
    if (d < 0) return false;

    Vec3f iPoint = ray.org+ray.dir*d - _c;
    if (_infinite || (abs(iPoint.x) < _w && abs(iPoint.z) < _w && abs(iPoint.y) < _w))
    {
      if (!ray.t(d)) return false; 
      ray.obj = this;
      ray.normal = _n;
      return true;
    }
    return false;
  }

  void Plane::draw(Color color)
  {
    glPushMatrix();
    glColor3f( color.x, color.y, color.z);
    glTranslatef(_c[0], _c[1], _c[2]);

    Vec3f y = Vec3f(0, 1, 0);
    Vec3f n = _n.normalized();
    Vec3f rotaxis = n.cross(y); 
    float rotangle = 180.0f * acos(y*n) / M_PI;
    glRotatef(rotangle, rotaxis[0], rotaxis[1], rotaxis[2]);

    glBegin(GL_QUADS);
    glNormal3fv(y.p());
    glVertex3f(_w, 0, _w);
    glVertex3f(_w, 0, -_w);
    glVertex3f(-_w, 0, -_w);
    glVertex3f(-_w, 0, _w);
    glEnd();

    glPopMatrix();        
  }

}
