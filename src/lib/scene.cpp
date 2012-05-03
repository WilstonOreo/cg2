#include <cmath>

#include "scene.hpp"

namespace cg2
{
  vec3f Scene::traceRay(Ray& ray, Object* curObj)
  {
    vector<Object*>::iterator objIt;
    for (objIt = objects.begin(); objIt != objects.end(); ++objIt)
    {
      if (*objIt == curObj) continue; 
      (*objIt)->intersect(ray);
    }

    vec3f color;
    if (ray.obj && ray.obj != curObj)
    {
      vec3f iPoint = ray.getIntersectionPoint();
      if (ray.obj->shader)
        color = ray.obj->shader->shade(ray);
      else
        color = vec3f(1.0,1.0,1.0);
    }
    return color;
  }


  bool Scene::traceShadowRay(Ray& shadowRay, Object* curObj)
  {
    vector<Object*>::iterator objIt;

    for (objIt = objects.begin(); objIt != objects.end(); ++objIt)
    {
      if (*objIt == curObj) continue;
      (*objIt)->intersect(shadowRay);
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
    FrameBuffer frameBuffer(outImage._w,outImage._h);
    vector<Ray>::iterator rayIt;
    for (rayIt = rays.begin(); rayIt != rays.end(); ++rayIt)
      frameBuffer.set(rayIt->scrPosX,rayIt->scrPosY,traceRay(*rayIt));
    frameBuffer.drawToImage(outImage);
  }

}

