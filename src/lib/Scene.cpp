#include <cmath>

#include <boost/foreach.hpp>

#include "cg2/Scene.hpp"

namespace cg2
{
  Vec3f Scene::traceRay(Ray& ray, IntersectableObject* curObj)
  {
    vector<Object*>::iterator objIt;

    BOOST_FOREACH( Object* obj, objects )
    {
      if (obj == curObj || obj->type() != OBJ_INTERSECT ) continue;
      ((IntersectableObject*)obj)->intersect(ray);
    }

    Vec3f color;
    if (ray.obj && ray.obj != curObj)
    {
      if (ray.obj->shader())
        color = ray.obj->shader()->shade(ray);
      else
        color = Vec3f(1.0,1.0,1.0);
    }
    return color;
  }


  bool Scene::traceShadowRay(Ray& shadowRay, IntersectableObject* curObj)
  {
    BOOST_FOREACH( Object* obj, objects )
    {
      if (obj == curObj) continue;
      ((IntersectableObject*)obj)->intersect(shadowRay);
      if (shadowRay.obj) return true;
    }
    return false;
  }
  void Scene::castRays(vector<Ray>& rays)
  {
    vector<Ray>::iterator rayIt;
    for (rayIt = rays.begin(); rayIt != rays.end(); ++rayIt)
      frameBuf.set(rayIt->scrPosX,rayIt->scrPosY,traceRay(*rayIt));
  }

  void Scene::castRays(vector<Ray>& rays, Image& outImage)
  {
    FrameBuffer frameBuffer(outImage.width(),outImage.height());
    vector<Ray>::iterator rayIt;
    for (rayIt = rays.begin(); rayIt != rays.end(); ++rayIt)
      frameBuffer.set(rayIt->scrPosX,rayIt->scrPosY,traceRay(*rayIt));
    frameBuffer.drawToImage(outImage);
  }

}

