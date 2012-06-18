#include "cg2/BoundingBox.hpp"

#include <GL/gl.h>
#include <tbd/log.h>

using namespace std;

namespace cg2
{
  bool BoundingBox::pointInBox(Point3f const & p) const
  {
    return p.x > min.x && p.x < max.x &&
           p.y > min.y && p.y < max.y &&
           p.z > min.z && p.z < max.z;
  }

  bool BoundingBox::intersect(Ray & ray) const
  {
    float tmin, tmax, tymin, tymax, tzmin, tzmax;

    float minV = min.x, maxV = max.x;
    if (ray.dir.x < 0) swap(minV,maxV);

    tmin = (minV - ray.org.x) / ray.dir.x;
    tmax = (maxV - ray.org.x) / ray.dir.x;

    minV = min.y, maxV = max.y;
    if (ray.dir.y < 0) swap(minV,maxV);

    tymin = (minV - ray.org.y) / ray.dir.y;
    tymax = (maxV - ray.org.y) / ray.dir.y;

    if ( (tmin > tymax) || (tymin > tmax) )  return false;

    if (tymin > tmin) tmin = tymin;
    if (tymax < tmax) tmax = tymax;

    minV = min.z, maxV = max.z;
    if (ray.dir.z < 0) swap(minV,maxV);

    tzmin = (minV - ray.org.z) / ray.dir.z;
    tzmax = (maxV - ray.org.z) / ray.dir.z;

    if ( (tmin > tzmax) || (tzmin > tmax) ) return false;

    if (tzmin > tmin) tmin = tzmin;
    if (tzmax < tmax) tmax = tzmax;

    if ((tmin < ray.tmax) && (tmax > ray.tmin))
    {
      ray.tmin = tmin;
      ray.tmax = tmax;
      return true;
    }
    return false;
  }

  Axis BoundingBox::dominantAxis() const
  {
    Vec3f _size = size();
    if (_size.x > _size.y)
    {
      if (_size.x > _size.z) return X;
    }
    else if (_size.y > _size.z) return Y;
    return Z;
  }

  void BoundingBox::split(float splitPos, Axis const & axis, BoundingBox & boxLeft, BoundingBox & boxRight) const
  {
    boxLeft.set(min,max);
    boxRight.set(min,max);
    boxLeft.max.cell[axis] = splitPos;
    boxRight.min.cell[axis] = splitPos;
  }

  vector<Point3f> BoundingBox::corners() const
  {
    vector<Point3f> _corners(8);
    _corners[0] = min;
    _corners[1] = Point3f(min.x,min.y,max.z);
    _corners[2] = Point3f(min.x,max.y,min.z);
    _corners[3] = Point3f(min.x,max.y,max.z);
    _corners[4] = Point3f(min.x,min.y,min.z);
    _corners[5] = Point3f(max.x,min.y,max.z);
    _corners[6] = Point3f(max.x,max.y,min.z);
    _corners[7] = max;
    return _corners;
  }

  void BoundingBox::draw(Color const & color) const
  {
    float x  = min.x, y  = min.y, z  = min.z;
    float xs = max.x, ys = max.y, zs = max.z;


    glColor3f(color.x,color.y,color.z);

    glBegin(GL_LINE_LOOP);
    // top side
    glVertex3f(x , ys, z);
    glVertex3f(xs, ys, z);
    glVertex3f(xs, ys, zs);
    glVertex3f(x , ys, zs);
    glEnd();

    glBegin(GL_LINE_LOOP);
    // bottom side
    glVertex3f(x , y, z);
    glVertex3f(xs, y, z);
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
    glVertex3f(x     , y     , z);
    glVertex3f(x     , y     , zs);
    glVertex3f(x     , ys, zs);
    glVertex3f(x     , ys, z);
    glEnd();

    glBegin(GL_LINE_LOOP);
    // south side
    glVertex3f(xs, y     , z);
    glVertex3f(xs, y     , zs);
    glVertex3f(xs, ys, zs);
    glVertex3f(xs, ys, z);
    glEnd();
  }

}
