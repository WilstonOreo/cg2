#pragma once

#include "Primitive.hpp"

namespace cg2 {
	struct BoundingBox : public Primitive {
		bool intersect(Ray & ray) const;
		void set(const Point3f & _min, const Point3f & _max) {
			min = _min;
			max = _max;
			for (int i = 0; i < 3; i++)
				if (min.cell[i] > max.cell[i]) {
					std::swap(min.cell[i],max.cell[i]);
				}
		}
		void draw(Color const & color = Color()) const;

		void split(float splitPos, Axis const & axis, BoundingBox & boxLeft, BoundingBox & boxRight) const;

		Vec3f normal(const Ray & ray) const {
			return Vec3f();
		}
		TexCoords texCoords(const Ray & ray) const {
			return TexCoords();
		}

		Axis dominantAxis() const;

		bool pointInBox(Point3f const & p) const;


		Vec3f size() const {
			return max-min;
		}

		// Bounding box points
		Point3f min, max;
	};
}
