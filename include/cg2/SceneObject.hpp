#include "cg2/Shader.hpp"
#include "cg2/BoundingBox.hpp"

namespace cg2
{
  struct SceneObject : public Primitive
  {
    bool visible;
    BoundingBox boundingBox_;
    TBD_PROPERTY_REF(Matrix4f,matrix);
    

  protected:
    virtual void calcBoundingBox() = 0;
  };

  DEFINE_CONTAINERS(SceneObject)
}
