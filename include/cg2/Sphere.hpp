#include "cg2/SceneObject.hpp"

namespace cg2 {
	struct Sphere : public SceneObject {
		Sphere(const Point3f & _center, float _radius)
			: center_(_center), radius_(_radius) {
		}

		bool intersect(Ray & ray);

		Vec3f normal(const Point3f & iPoint);
		TexCoords texCoords(const Point3f & iPoint);

		void draw();

		TBD_DECLARE_PROPERTY_REF(Point3f,center);
		TBD_DECLARE_PROPERTY(float,radius);
	};

}
