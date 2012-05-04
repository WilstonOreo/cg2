#ifndef _SCENE_HPP
#define _SCENE_HPP

#include "cg2/Object.hpp"
#include "cg2/Light.hpp"
#include "cg2/Camera.hpp"
#include "cg2/FrameBuffer.hpp"

#include <vector>

namespace cg2
{
  using std::vector;

  struct Scene
  {
    Scene() { maxBounce = 15; }

    void addObject(Object* obj) { objects.push_back(obj); }

    void castRays(vector<Ray>& rays, Image& outImage);
    void castRays(vector<Ray>& rays);

    Vec3f traceRay(Ray& ray, IntersectableObject* curObj = NULL);
    bool traceShadowRay(Ray& shadowRay, IntersectableObject* curObj = NULL);

    unsigned maxBounce;

    vector<Object*> objects;
    vector<Light*>   lights;
    Camera* camera;
    FrameBuffer frameBuf;
  };
}

#endif
