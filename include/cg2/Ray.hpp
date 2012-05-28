#pragma once
#include "cg2/Vector.hpp"

#include <vector>

namespace cg2 {
	class Primitive;

	struct Ray {
		Ray(Point3f _org = Point3f(), Vec3f _dir = Vec3f(), float _scrPosX = 0.0, float _scrPosY = 0.0):
			org(_org), dir(_dir) {
			tmin = 0.002;
			tmax = 10000.0;
			obj = NULL;
			bounce = 0;
			u = 0.0;
			v = 0.0;
			scrPosX = _scrPosX;
			scrPosY = _scrPosY;
		}

		void init(Point3f _org = Point3f(), Vec3f _dir = Vec3f(), float _scrPosX = 0, float _scrPosY = 0) {
			org = _org;
			dir = _dir;
			setScreenPos(_scrPosX,_scrPosY);
		}

		bool t(float _t) {
			if (_t >= tmin && _t < tmax) {
				tmax = _t;
				return true;
			}
			return false;
		}

		string toString();

		Ray reflect();
		Ray refract(float index);

		void texCoord(float _u, float _v) {
			u =_u;
			v =_v;
		}
		void setScreenPos(float _scrPosX, float _scrPosY) {
			scrPosX = _scrPosX;
			scrPosY = _scrPosY;
		}
		void drawIntersectionPoint();
		Point3f getIntersectionPoint() {
			return org + tmax*dir;
		}

		Point3f org;
		Vec3f dir;
		float tmin, tmax;
		Vec3f normal,color;
		Primitive * obj;
		unsigned bounce;

		float u,v;

		float scrPosX, scrPosY;
	};

	typedef enum { DIR_TOP, DIR_BOTTOM, DIR_LEFT, DIR_RIGHT, DIR_FRONT, DIR_BACK } Direction;

	typedef std::vector<Ray *> RayList;
	typedef std::vector<Ray> Rays;

}

