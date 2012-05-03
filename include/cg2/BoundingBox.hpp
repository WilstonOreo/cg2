#include "Object.hpp"

namespace cg2 
{
  struct BoundingBox : public IntersectableObject
  {
    void intersect(Ray& ray, float& tnear, float& tfar);
    void set(const vec3f& _min, const vec3f& _max) { min = _min; max = _max; }
    void draw(Color color = Color());

    int dominantAxis() 
    {
      vec3f d = min - max; d.set(abs(d.x),abs(d.y),abs(d.z));
      if (d.x > d.y)
      { 	if (d.x > d.z) return 0;
      } else
        if (d.y > d.z) return 1;
      return 2;
    }

    float split(float splitPos, int axis, BoundingBox& boxLeft, BoundingBox& boxRight)
    {
      for (int i = 0; i < 3; i++)
        if (min.cell[i] > max.cell[i]) swap(min.cell[i],max.cell[i]);

      boxLeft.set(min,max);
      boxRight.set(min,max);
      boxLeft.max.cell[axis] = splitPos;
      boxRight.min.cell[axis] = splitPos;
    }

    // Bounding box points
    point3f min, max;
  };


}
