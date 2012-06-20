#pragma once

#include "cg2/Vertex.hpp"
#include "cg2/Compound.hpp"

#include <map>
#include <set>

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

  class PointCloud : public Compound<Vertex>
  {
    public:
      PointCloud();

      void read(const string& filename);
      void write(const string& filename) const;

      bool intersect(Ray& _ray, Vec3f* _normal = NULL, Point2f* _texCoords = NULL) const { return false; }

      void update();
      virtual std::set<const Vertex*> collectKNearest(Point3f const & p, int k) const;
      virtual std::set<const Vertex *> collectInRadius(Point3f const & p, float radius) const;
      bool isNearest(const Vertex& _v, const Point3f& _p) const;

      set<const Vertex*> selection;

      const std::vector<Vertex>& vertices() const { return Compound<Vertex>::objs_; }

      void draw(const Color4f& _color = Color4f()) const;

      void collect(KDNode<Vertex>* node, const BoundingBox& box, PointSet& pointSet) const;

      TBD_PROPERTY(bool,drawKDTree);
      TBD_PROPERTY(bool,drawBoundingBox);
      TBD_PROPERTY(Color4f,boundingBoxColor);
      TBD_PROPERTY(Color4f,selectionColor);
    
    private:
      virtual void divideNode(KDNode<Vertex>* node, const BoundingBox& box, int depth);
      float nodeDistance(const Point3f& p, const BoundingBox& box) const;
  };
}
