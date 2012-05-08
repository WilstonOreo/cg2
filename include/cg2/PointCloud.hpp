#pragma once

#include "cg2/Mesh.hpp"
#include "cg2/KDTree.hpp"

#include <set>

namespace cg2
{
  struct SelectedPoint
  {
    SelectedPoint(float _dist, Vertex* _v) : dist(_dist), v(_v) {}
    float dist;
    Vertex* v;
  };

  struct PointCompare
  {
    bool operator() (const SelectedPoint& a, const SelectedPoint& b) 
    {
      return a.dist < b.dist;
    }
  };

  class PointSet : public std::set<SelectedPoint,PointCompare>
  {
    public:
    PointSet(Point3f _center);

    bool insert(Vertex* v,float maxDist = 0.0f, unsigned k = 0);
    VertexSet vertexSet();

    TBD_DECLARE_PROPERTY(Point3f,center);
  };

  class PointKDTree : public KDTree<Vertex>
  {
  public:
    void collectKNearest(Point3f& p, int k, KDNode<Vertex>* node, PointSet& pointSet);
    void collectInRadius(Point3f& p, float radius, KDNode<Vertex>* node, PointSet& pointSet);
    
  private:
    void divideNode(KDNode<Vertex>* node, BoundingBox& box, int depth);
  };


  class PointCloud : public Mesh
  {
    public:
      void read(string filename);
      void write(string filename);

      void draw(Color color = Color());

      Vec3f normal(const Ray& ray) { return Vec3f(); }
      TexCoords texCoords(const Ray& ray) { return TexCoords(); }

      bool intersect(Ray& ray) { return false; }


      void update();
      void collectKNearest(Point3f& p, int k);
      void collectInRadius(Point3f& p, float radius);

      VertexSet selection;

      TBD_DECLARE_PROPERTY(bool,drawKDTree);
      TBD_DECLARE_PROPERTY(bool,highlightSelection);
      TBD_DECLARE_PROPERTY(Color,selectionColor);
    private:
      PointKDTree kdTree;
  };
}
