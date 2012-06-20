#pragma once 
#include "cg2/Vector.hpp"

#include <vector>

namespace cg2
{
  struct Primitive;

  struct Ray
  {
    Ray(const Point3f _org = Point3f(), const Vec3f _dir = Vec3f(), float _tMin = 0.002, float _tMax = INF): 
      org_(_org), dir_(_dir), primitive_(NULL), tMin_(_tMin), tMax_(_tMax) {  }

    bool intersection(Primitive* _primitive, float _t) 
    {
      if (_t >= tMin_ && _t < tMax_)
      {
      primitive_ = _primitive;
      tMin_ = _t;
      return true;
      }
      return false;
    }

    Point3f intersectionPoint() const
    {
      return org_ + dir_ * tMin_;
    }

    void params(Point3f _org, Vec3f _dir) 
    {
      org_=_org; dir_=_dir;
    }
    Ray reflect();
    Ray refract(float index);

    Point3f org_;
    Vec3f dir_;
   
    Primitive* primitive_;
    float tMin_, tMax_;
  };

  typedef std::vector<Ray*> RayList;
  typedef std::vector<Ray> Rays;

}

