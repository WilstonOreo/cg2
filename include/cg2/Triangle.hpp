#pragma once

#include "cg2/Vertex.hpp"
#include "cg2/Primitive.hpp"

namespace cg2
{
  template<class POINT>
  struct TriangleConcept : public Primitive
  {    
    virtual const Point3f& v0() const = 0;
    virtual const Point3f& v1() const = 0;
    virtual const Point3f& v2() const = 0;
    virtual const Vec3f normal(Point2f* _texCoords = NULL) const = 0;

    virtual bool intersect(Ray& _ray, Vec3f* _normal = NULL, Point2f* _texCoords = NULL) const 
    {
      Vec3f A = v1() - v0(), 
            B = v2() - v0();
      Vec3f p = _ray.dir_.cross(B);

      // d = Determinant
      float d = A.dot(p);
      if (d > -EPSILON && d < EPSILON) return false;

      float inv_d = 1.0f / d;

      Vec3f tV = _ray.org_ - v0();

      float u = tV.dot(p) * inv_d;
      if (u < 0.0 || u > 1.0) return false;

      Vec3f q = tV.cross(A);
      float v = _ray.dir_.dot(q) * inv_d;
      if (v < 0.0 || u + v > 1.0) return false;

      float t = B.dot(q) *  inv_d;

      if (_ray.intersection(this->pointer(),t))
      {
        if (_texCoords) (*_texCoords)(u,v);
        if (_normal) (*_normal)(normal(_texCoords));
        return true;
      }
      return false;
    }  

    int intersect(float splitPos, int axis)
    {
      float minPos = std::min(v0()[axis],std::min(v1()[axis],v2()[axis]));
      float maxPos = std::max(v0()[axis],std::max(v1()[axis],v2()[axis]));

      if (maxPos < splitPos) return 1;
      if (minPos > splitPos) return 2; 
      return 3;
    }

    Bounds bounds() const
    {
      return Bounds(Point3f(std::min(v0().x(),std::min(v1().x(),v2().x())),
                            std::min(v0().y(),std::min(v1().y(),v2().y())),
                            std::min(v0().z(),std::min(v1().z(),v2().z()))),
                    Point3f(std::max(v0().x(),std::max(v1().x(),v2().x())),
                            std::max(v0().y(),std::max(v1().y(),v2().y())),
                            std::max(v0().z(),std::max(v1().z(),v2().z()))));
    }

    POINT v[3];
  };

  struct Triangle : public TriangleConcept<Point3f>
  {
    Triangle() 
    {
      v[0](Point3f()); 
      v[1](Point3f()); 
      v[2](Point3f()); 
      n_(Vec3f());
    }

    Triangle(Point3f _v0, Point3f _v1, Point3f _v2, Vec3f _n = Vec3f())
    {
      v[0] = _v0; v[1] = _v1; v[2] = _v2; n_ = _n;
      if (_n.sqrLength() == 0.0 )
        n_ = (v[2] - v[0]).cross(v[1] - v[0]);
    }

    const Point3f& v0() const { return v[0]; }
    const Point3f& v1() const { return v[1]; }
    const Point3f& v2() const { return v[2]; }
    const Vec3f normal(Point2f* _texCoords = NULL) const { return n_; }

    void draw(const Color4f& _color = Color4f()) const;

    private:
      Vec3f n_;
  };

  struct VertexTriangle : public TriangleConcept<Vertex*>
  {
    VertexTriangle(Vertex* _v0, Vertex* _v1, Vertex* _v2)
    {
      v[0] = _v0; v[1] = _v1; v[2] = _v2; 
    }

    const Point3f& v0() const { return v[0]->v; }
    const Point3f& v1() const { return v[1]->v; }
    const Point3f& v2() const { return v[2]->v; }
    const Vec3f normal(Point2f* _texCoords = NULL) const 
    { 
      if (_texCoords)
          return (1.0 - _texCoords->x() - _texCoords->y())*v[0]->n + 
                  _texCoords->x()*v[1]->n + _texCoords->y()*v[2]->n;
      return v[0]->n; 
    }
    
    void draw(const Color4f& _color = Color4f()) const;
  };

}
