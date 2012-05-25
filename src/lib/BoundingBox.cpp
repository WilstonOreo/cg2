#include "cg2/BoundingBox.hpp"

#include <GL/gl.h>

using namespace std;

namespace cg2 
{
  bool BoundingBox::pointInBox(Point3f p)
  {
    return p.x > min.x && p.x < max.x &&
      p.y > min.y && p.y < max.y &&
      p.z > min.z && p.z < max.z;
  }

  bool BoundingBox::intersect(Ray& ray)
  {
    float tnear = 1000.0, tfar = -1000.0;
    for (int i = 0; i < 3; i++)
    {
      if (abs(ray.dir[i]) < 0.001) continue;
      float minV = min.cell[i], maxV = max.cell[i];
      if (ray.dir[i] < 0) swap(minV,maxV);

      float tn = (minV - ray.org.cell[i]) / ray.dir.cell[i];
      float tf = (maxV - ray.org.cell[i] ) / ray.dir.cell[i];

      if (tn < tnear) tnear = tn;
      if (tf > tfar) tfar = tf;
    }

    if (tnear < tfar)
    {
      ray.tmin = tnear;
      ray.tmax = tfar;
    }
    return (tnear < tfar);
  }

  Axis BoundingBox::dominantAxis() 
  {
    Vec3f d = min - max; d.set(abs(d.x),abs(d.y),abs(d.z));
    if (d.x > d.y)
    { 	if (d.x > d.z) return X;
    } else
      if (d.y > d.z) return Y;
    return Z;
  }

  void BoundingBox::split(float splitPos, Axis axis, BoundingBox& boxLeft, BoundingBox& boxRight)
  {
    for (int i = 0; i < 3; i++)
      if (min.cell[i] > max.cell[i]) swap(min.cell[i],max.cell[i]);

    boxLeft.set(min,max);
    boxRight.set(min,max);
    boxLeft.max.cell[axis] = splitPos;
    boxRight.min.cell[axis] = splitPos;
  }

  void BoundingBox::draw(Color color)
  {
    float x  = min.x, y  = min.y, z  = min.z;
    float xs = max.x, ys = max.y, zs = max.z;


    glColor3f(color.x,color.y,color.z);

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
