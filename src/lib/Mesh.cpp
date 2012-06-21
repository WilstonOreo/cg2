#include "cg2/Mesh.hpp"

#include <boost/foreach.hpp>
#include "cg2/OFFReader.hpp"

#include <tbd/log.h>

#include <GL/glu.h>

using namespace std;

namespace cg2
{
  void VertexMesh::draw(const Color4f& _color) const
  {
    BOOST_FOREACH ( const VertexTriangle& tri, objs_ )
      tri.draw(_color);
  }
  
  void TriangleMesh::read(const string& filename)
  {
    OFFReader off;
    Vertices vertices;
    off.read(filename,&vertices,&objs_);
    calcBoundingBox();
    build(objs_,boundingBox_);
  }

  void TriangleMesh::draw(const Color4f& _color) const
  {
    BOOST_FOREACH ( const Triangle& tri, objs_ )
      tri.draw(_color);
  }


  std::pair<TriangleMesh,TriangleMesh> TriangleMesh::split(const Plane& splitPlane)
  {
    std::pair<TriangleMesh,TriangleMesh> halves;
    
    BOOST_FOREACH ( Triangle& tri, objs_ )
      splitTriangle(tri,splitPlane,halves);

    halves.first.calcBoundingBox();
    halves.first.build(halves.first.objs_,halves.first.boundingBox_);    
    halves.second.calcBoundingBox();
    halves.second.build(halves.second.objs_,halves.first.boundingBox_);

    return halves;
  }

  void TriangleMesh::splitTriangle(const Triangle& tri, const Plane& plane, 
                std::pair<TriangleMesh,TriangleMesh>& _halves)
  {
    vector<Triangle> triangles;
    Point3f V[3]; 
    for (int i = 0; i < 3; i++)
      V[i] = tri.v[i];

    int signCount = 0;
    bool signs[3];

    for (int i = 0; i < 3; i++)
    {
      signs[i] = (V[i] - plane.center_).dot(plane.normal_);
      signCount += int(signs[i] < 0);
    }

    vector<Triangle> *q = &_halves.first.objs_, *r = &_halves.second.objs_;
    if (signCount >= 2) swap(q,r);

    if (signCount == 0 || signCount == 3) 
    {
      r->push_back(tri);
      return;
    }

    int k = 0;
    for (int i = 1; i < 3; i++)
      if ((signCount == 1) ? signs[i] : !signs[i]) k = i;
    int u = (k+1) % 3, v = (k+2) % 3;

    Vec3f A = V[u] - V[k], B = V[v] - V[k]; 
    Point3f iPoint[2];
    iPoint[0] = V[k] + A*(plane.normal_.dot(plane.center_ - V[k]) / A.dot(plane.normal_));
    iPoint[1] = V[k] + B*(plane.normal_.dot(plane.center_ - V[k]) / B.dot(plane.normal_));

    q->push_back(Triangle(V[k],iPoint[0],iPoint[1],tri.normal()));
    r->push_back(Triangle(V[u],V[v],iPoint[0],tri.normal()));
    r->push_back(Triangle(V[u],iPoint[0],iPoint[1],tri.normal()));
  }

}
