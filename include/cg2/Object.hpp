#include "cg2/Vector.hpp"
#include "cg2/Shader.hpp"
#include "cg2/Ray.hpp"

#include "cg2/misc.hpp"

namespace cg2 
{
  struct Object
  {
    virtual void draw(Color color = Color()) = 0;
    bool visible;
  };

  struct ShadableObject : public Object
  {
    virtual vec3f getNormal(const vec3f& iPoint) = 0;
    virtual pair<float,float> getTexCoords(const vec3f& iPoint) = 0;

    Shader* shader;
  };

  struct IntersectableObject: public ShadableObject
  {
    virtual bool intersect(Ray& ray) = 0;
  };

}
