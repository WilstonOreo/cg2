#include <cmath>

#include <boost/foreach.hpp>

#include "cg2/Scene.hpp"

namespace cg2
{
  Vec3f Scene::traceRay(Ray& ray, SceneObject* curObj)
  {
/*
    vector<Object*>::iterator objIt;

    BOOST_FOREACH( Object* obj, objects )
    {
      if (obj == curObj || obj->type() != OBJ_INTERSECT ) continue;
      ((IntersectableObject*)obj)->intersect(ray);
    }
    Vec3f color;
    if (ray.obj && ray.obj != curObj)
    {
      color = Vec3f(1.0,1.0,1.0);  
      if (ray.obj->type() == OBJ_SHADABLE)
        if (((ShadableObject*)ray.obj)->shader())
          color = ((ShadableObject*)ray.obj)->shader()->shade(ray);
    }
    return color;*/
    return Vec3f();
  }


  bool Scene::traceShadowRay(Ray& shadowRay, SceneObject* curObj)
  {
    BOOST_FOREACH( SceneObject* obj, objects )
    {
      if (obj == curObj) continue;
      obj->intersect(shadowRay);
      if (shadowRay.obj) return true;
    }
    return false;
  }
  void Scene::castRays(Rays& rays, Image* outImage)
  {
    if (outImage) frameBuffer.resize(outImage->width(),outImage->height());

    BOOST_FOREACH( Ray& ray, rays)
      frameBuffer.set(ray.scrPosX,ray.scrPosY,traceRay(ray));

    if (outImage) frameBuffer.drawToImage(*outImage);

  }
}

