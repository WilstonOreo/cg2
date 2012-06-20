#include "cg2/SceneObject.hpp"

namespace cg2 
{
	struct Sphere : public Primitive 
	{
		Sphere(const Point3f& _center, float _radius)
			: center_(_center), radius_(_radius)
		{}

    bool intersect(Ray& _ray, Vec3f* _normal = NULL, Point2f* _texCoords = NULL) const;
	
    Bounds bounds() const;

    TBD_PROPERTY_REF(Point3f,center);
    TBD_PROPERTY(float,radius);
	};

}
