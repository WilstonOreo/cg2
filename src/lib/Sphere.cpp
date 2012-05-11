#include "cg2/Sphere.hpp"


#include <GL/glut.h>

namespace cg2 
{
	void Sphere::draw()
	{
		glPushMatrix();
		glTranslatef(center_[0], center_[1], center_[2]);
		glutSolidSphere(radius_, 32, 32);
		glPopMatrix();
	}

	bool Sphere::intersect(Ray& ray)
	{
		Vec3f o = ray.org - center();
		float a = ray.dir * ray.dir;
		float b = 2.0f * (ray.dir * o);
		float c = o*o - radius()*radius();
		float disc = b*b - 4*a*c;

		if (disc < 0) return false;

		float discSqrt = sqrtf(disc);
		float q = (b < 0) ? (-b - discSqrt) : (-b + discSqrt);
		q *= 0.5f;

		float t0 = q/a, t1 = c/q;
		if (t0 > t1) std::swap(t0,t1);
		if (t1 < 0) return false;

		if (!ray.t((t0 < 0) ? t1 : t0)) return false;

		ray.obj = this;
		ray.normal = normal(ray.getIntersectionPoint());
		return true;
	}

	Vec3f Sphere::normal(const Point3f& iPoint)
	{
		Vec3f n = iPoint - center();
		return n.normalized();
	}

	TexCoords Sphere::texCoords(const Point3f& iPoint)
	{
		// from http://www.cse.msu.edu/~cse872/tutorial4.html
		Vec3f n = normal(iPoint);
		float u = atan2(n.x,n.z) / (2.0*M_PI) + 0.5;
		float v = acosf(n.y)/M_PI;

		return TexCoords(u,v);
	}


}
