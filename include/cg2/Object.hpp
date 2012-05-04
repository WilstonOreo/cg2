#include "cg2/Vector.hpp"
#include "cg2/Shader.hpp"
#include "cg2/Ray.hpp"

#include "cg2/misc.hpp"

namespace cg2 
{
  typedef enum { OBJ_BASE, OBJ_SHADABLE, OBJ_INTERSECT } ObjectType;

  struct Object
  {
	ObjectType type() { return OBJ_BASE; }

    virtual void draw(Color color = Color()) = 0;
    bool visible;
  };

  struct ShadableObject : public Object
  {
	ObjectType type() { return OBJ_SHADABLE; }
    
	virtual Vec3f normal(const Point3f& iPoint) = 0;
    virtual TexCoords texCoords(const Point3f& iPoint) = 0;

	TBD_DECLARE_PROPERTY(Shader*,shader);
  };

  struct IntersectableObject: public ShadableObject
  {
	ObjectType type() { return OBJ_INTERSECT; }
    
	virtual bool intersect(Ray& ray) = 0;
  };

}
