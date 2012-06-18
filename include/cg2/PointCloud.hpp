#pragma once

#include "cg2/Mesh.hpp"
#include "cg2/KDTree.hpp"

#include <map>

namespace cg2
{
  class PointSet
  {
  public:
    std::multimap<double, Vertex *> points;
    PointSet(Point3f _center, float _radius = std::numeric_limits<float>::max(), int _k = std::numeric_limits<int>::max());

    virtual bool insert(Vertex * v);
    set<Vertex const *> vertexSet();
    float maxDist();

    TBD_PROPERTY(int,k)
    TBD_PROPERTY(float,radius)
    TBD_PROPERTY_REF(Point3f,center)
  };

  class PointKDTree : public KDTree<Vertex>
  {
  public:
    PointKDTree() : drawDepth_(10) {}
    virtual void collect(KDNode<Vertex> const * node, BoundingBox const & box, PointSet & pointSet) const;

    TBD_PROPERTY(unsigned,drawDepth)
  private:
    virtual void divideNode(KDNode<Vertex> * node, BoundingBox & box, int depth);

    virtual float nodeDistance(Point3f const & p, BoundingBox const & box) const;
  };


  class PointCloud : public Mesh
  {
  public:
    PointCloud();

    void read(string const & filename);
    void write(string const & filename) const;

    void draw(Color const & color = Color()) const;

    Vec3f normal(const Ray & ray) const
    {
      Q_UNUSED(ray);
      return Vec3f();
    }
    TexCoords texCoords(const Ray & ray) const
    {
      Q_UNUSED(ray);
      return TexCoords();
    }

    bool intersect(Ray & ray) const
    {
      Q_UNUSED(ray);
      return false;
    }

    virtual void update();
    virtual set<Vertex const *> collectKNearest(Point3f const & p, int k) const;
    virtual set<Vertex const *> collectInRadius(Point3f const & p, float radius) const;

    bool isNearest(const Vertex& _v, const Point3f& _p);


    set<Vertex const *> selection;

    TBD_PROPERTY(bool,drawKDTree);
    TBD_PROPERTY(Color,kdTreeColor);
    TBD_PROPERTY(bool,drawBoundingBox);
    TBD_PROPERTY(Color,boundingBoxColor);

    TBD_PROPERTY(bool,highlightSelection);
    TBD_PROPERTY(Color,selectionColor);
  private:
    PointKDTree kdTree;
  };
}
