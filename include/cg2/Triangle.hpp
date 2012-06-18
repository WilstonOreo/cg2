#pragma once

#include "cg2/Vertex.hpp"
#include "cg2/Primitive.hpp"

namespace cg2
{
  struct Triangle : public Primitive
  {
    Triangle(Vertex * _v0, Vertex * _v1, Vertex * _v2)
    {
      build(_v0,_v1,_v2);
    }

    Vertex * v0, *v1, *v2;

    bool intersect(Ray & ray);

    int splitPlaneIntersect(float splitPos, int axis);

    Vec3f normal(const Ray & ray);
    TexCoords texCoords(const Ray & ray)
    {
      Q_UNUSED(ray);
      return TexCoords();
    }

    void draw(Color color = Color())
    {
      Q_UNUSED(color);
    }

  private:
    void build(Vertex * _v0, Vertex * _v1, Vertex * _v2);

    float n_u, n_v, n_d;
    int   kuv;

    float b_nu, b_nv;
    float c_nu, c_nv, av, au;
  };

  typedef vector<Triangle> Triangles;
  typedef vector<Triangle *> TriangleList;
}
