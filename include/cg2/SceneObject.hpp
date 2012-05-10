#include "cg2/Shader.hpp"
#include "cg2/BoundingBox.hpp"

namespace cg2
{
  struct SceneObject : public Primitive
  {
    bool visible;
    TBD_DECLARE_PROPERTY_REF(Matrix4f,matrix);   
    TBD_DECLARE_PROPERTY_REF(BoundingBox,boundingBox);

  protected:
    virtual void calcBoundingBox() = 0;
  };

  DEFINE_CONTAINERS(SceneObject)
}
