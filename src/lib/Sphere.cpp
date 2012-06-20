#include "cg2/Sphere.hpp"

#include <GL/glut.h>
#include <cmath>

using namespace std; 

namespace cg2 
{
/*	void Sphere::draw(Color color) const
	{
		glPushMatrix();
		glTranslatef(center_[0], center_[1], center_[2]);
		glutSolidSphere(radius_, 32, 32);
		glPopMatrix();
	}
*/
	bool Sphere::intersect(Ray& _ray, Vec3f* _normal, Point2f* _texCoords) const
	{
		Vec3f o = _ray.org_ - center_;
		float a = _ray.dir_.dot( _ray.dir_ ) ;
		float b = 2.0f * (_ray.dir_.dot(o));
		float c = o.dot(o) - radius()*radius();
		float disc = b*b - 4*a*c;

		if (disc < 0) return false;

		float discSqrt = sqrtf(disc);
		float q = (b < 0) ? (-b - discSqrt) : (-b + discSqrt);
		q *= 0.5f;

		float t0 = q/a, t1 = c/q;
		if (t0 > t1) std::swap(t0,t1);
		if (t1 < 0) return false;

    if (_normal) 
    { 
      (*_normal)((_ray.intersectionPoint() - center_).normalized());
      if (_texCoords)
        (*_texCoords)(atan2(_normal->x(),_normal->z()) / (2.0*M_PI) + 0.5,acosf(_normal->y())/M_PI);
    }
    return _ray.intersection(pointer(),(t0 < 0) ? t1 : t0);
	}

  Bounds Sphere::bounds() const
  {
    Point3f _rP(radius_,radius_,radius_),
            _rM(-radius_,-radius_,-radius_);
    return Bounds(center_ + _rM, center_ + _rP);
  }

}
