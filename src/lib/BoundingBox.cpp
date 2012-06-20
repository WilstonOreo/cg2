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

  bool BoundingBox::intersect(Ray& _ray, Vec3f* _normal, Point2f* _texCoords) const
  {
    float tmin, tmax, tymin, tymax, tzmin, tzmax;

    float minV = min().x(), maxV = max().x();
    if (_ray.dir_.x() < 0) swap(minV,maxV);

    tmin = (minV - _ray.org_.x()) / _ray.dir_.x();
    tmax = (maxV - _ray.org_.x()) / _ray.dir_.x();

    minV = min().y(), maxV = max().y();
    if (_ray.dir_.y() < 0) swap(minV,maxV);

    tymin = (minV - _ray.org_.y()) / _ray.dir_.y();
    tymax = (maxV - _ray.org_.y()) / _ray.dir_.y();

    if ( (tmin > tymax) || (tymin > tmax) )  return false;

    if (tymin > tmin) tmin = tymin;
    if (tymax < tmax) tmax = tymax;

    minV = min().z(), maxV = max().z();
    if (_ray.dir_.z() < 0) swap(minV,maxV);

    tzmin = (minV - _ray.org_.z()) / _ray.dir_.z();
    tzmax = (maxV - _ray.org_.z()) / _ray.dir_.z();

    if ( (tmin > tzmax) || (tzmin > tmax) ) return false;

    if (tzmin > tmin) tmin = tzmin;
    if (tzmax < tmax) tmax = tzmax;

    if ((tmin < _ray.tMax_) && (tmax > _ray.tMin_))
    {
      _ray.tMin_ = tmin;
      _ray.tMax_ = tmax;
      return true;
    }
    return false;  
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
