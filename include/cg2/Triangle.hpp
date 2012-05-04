#pragma once

#include "cg2/Object.hpp"

namespace cg2
{
  struct Triangle : public IntersectableObject
  {
    Triangle(Vertex* _v0, Vertex* _v1, Vertex* _v2)
    {
      build(_v0,_v1,_v2);
    }
    float n_u, n_v, n_d;
    int   kuv;

    float b_nu, b_nv;
    float c_nu, c_nv, av, au;

    Vertex *v0, *v1, *v2;

    bool intersect(Ray &ray);

    int splitPlaneIntersect(float splitPos, int axis);

    Vec3f normal(const Vec3f& iPoint) {};
    TexCoords texCoords(const Point3f& iPoint) {};

    void draw() {}

    Vec3f normal(Ray& ray)
    {
      Vec3f n = (1.0f - ray.u - ray.v)*v0->n + ray.u*v1->n + ray.v*v2->n;
      return n.normalized();
    }

    private:
    void build(Vertex* _v0, Vertex* _v1, Vertex* _v2);
  };
}
