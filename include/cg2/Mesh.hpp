#pragma once

#include "cg2/Compound.hpp"
#include "cg2/Triangle.hpp"
#include <OpenMesh/Core/IO/MeshIO.hh>
#include <OpenMesh/Core/Mesh/TriMesh_ArrayKernelT.hh>

namespace cg2
{
  class ImplicitSurface;

  struct MeshTraits
  {
    typedef Point3f Point;
    typedef Vec3f   Normal;
    typedef float   TexCoord1D;
    typedef Vec2f   TexCoord2D;
    typedef Vec3f   TexCoord3D;
    typedef size_t  TextureIndex;
    typedef Vec3f Color;

    VertexTraits {};
    HalfedgeTraits {};
    EdgeTraits {};
    FaceTraits {};
    VertexAttributes(0);
    HalfedgeAttributes(OpenMesh::Attributes::PrevHalfedge);
    EdgeAttributes(0);
    FaceAttributes(OpenMesh::Attributes::Normal);
  };

  class Mesh : public OpenMesh::TriMesh_ArrayKernelT<MeshTraits> /*, public Compound<Triangle>*/
  {
  public:
    void read(string _filename);

    void optimize(ImplicitSurface& _implicitSurface, unsigned _nVertices = 0, float _quality = 0);
    void draw(const Color4f& color = Color4f()) const;

    void simplify();
    void refine();

    float cost(const ImplicitSurface& _implicitSurface) const;

    Bounds& bounds() { return bounds_; }

  protected:
    void divideNode(KDNode<Triangle>* node, const BoundingBox& box, int depth) {}

  private:
    float cost(const ImplicitSurface& _implicitSurface, const Triangle& _triangle) const;
  
    Bounds bounds_;
  };

}
