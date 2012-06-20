#include "cg2/BoundingBox.hpp"

namespace cg2
{
  /* A SceneObject is an object which is transformable and displayable within a scene.
     Moreover, it has a bounding box to determine its extents.
   */
  struct SceneObject : public Primitive
  {
    bool visible;
    TBD_PROPERTY_REF(Matrix4f,matrix);   
  protected:
    virtual void calcBoundingBox() = 0;
  };
}
