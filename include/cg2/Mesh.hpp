#pragma once

#include <map>

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
    VertexAttributes(OpenMesh::Attributes::Normal | OpenMesh::Attributes::Status);
    HalfedgeAttributes(OpenMesh::Attributes::Normal | OpenMesh::Attributes::PrevHalfedge | OpenMesh::Attributes::Status);
    EdgeAttributes(OpenMesh::Attributes::Status);
    FaceAttributes(OpenMesh::Attributes::Normal | OpenMesh::Attributes::Status);
  };

  class Mesh : public OpenMesh::TriMesh_ArrayKernelT<MeshTraits> /*, public Compound<Triangle>*/
  {
  public:
    typedef std::multimap<float,Mesh::FaceHandle> FaceCostMap;
    typedef std::multimap<float,Mesh::VertexHandle> VertexCostMap;

    void read(string _filename);

    void optimize(ImplicitSurface& _implicitSurface, unsigned _nVertices = 0, float _quality = 0);
    void draw(const Color4f& color = Color4f()) const;

    void simplify();
    void refine();

    float cost(const ImplicitSurface& _implicitSurface) const;

    float cost(const Mesh::VertexHandle& _vHandle); 

    Bounds& bounds() { return bounds_; }

  private:
    float cost(const ImplicitSurface& _implicitSurface, const Mesh::FaceHandle& _fHandle) const;
  
    Bounds bounds_;
  };

}
