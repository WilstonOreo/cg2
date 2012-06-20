#pragma once

#include "cg2/Primitive.hpp"

#include <vector>
#include <set>

namespace cg2 
{
  using std::vector;
  using std::set;

  struct Vertex : public Primitive
  {
    Vertex(Point3f _v = Point3f(), Vec3f _n = Vec3f()) { v(_v); n(_n); }
    void set(Point3f _v, Vec3f _n = Vec3f()) { v = _v; n = _n; }

    bool intersect(Ray& _ray, Vec3f* _normal = NULL, Point2f* _texCoords = NULL) const 
    {
        return false;
    }

    Bounds bounds() const { return Bounds(v,v); }

    void draw(const Color4f& _color = Color4f()) const;

    Point3f v;
    Vec3f n;
    bool hasNormal() { return (n.length() != 0.0f); }

  };

  typedef vector<Vertex> Vertices;
  typedef vector<Vertex*> VertexList;
  typedef std::set<Vertex*> VertexSet;

  struct Polygon : public VertexList
  {
    Polygon(int n = 3) { if (n < 3) n = 3; resize(n); } 
	  Vec3f normal();
  };

  typedef vector<Polygon*> PolygonList;
  typedef vector<Polygon> Polygons;
}
