#pragma once

#include "cg2/Primitive.hpp"

namespace cg2 {
	struct Plane : public Primitive {
		bool _infinite;
		Point3f _c;
		Vec3f _n;
		float _w;

		Plane(const Point3f & center, const Vec3f & normal, float width)
			: _c(center), _n(normal), _w(width) {

		}

		bool intersect(Ray & ray);
		Vec3f normal(const Vec3f & iPoint) {
			return _n;
		}

		void draw(Color color = Color());
	};
}
