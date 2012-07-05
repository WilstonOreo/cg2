#include "cg2/Mesh.hpp"

#include <boost/foreach.hpp>
#include "cg2/OFFReader.hpp"

#include <fstream>
#include <boost/algorithm/string/trim.hpp>
#include <boost/algorithm/string/split.hpp>

#include "cg2/ImplicitSurface.hpp"

#include <tbd/log.h>

#include <GL/glu.h>

using namespace std;

namespace cg2
{

  void Mesh::read(string _filename)
  {
    if (!OpenMesh::IO::read_mesh(*this, _filename))
    {
      LOG_ERR << "Read error.";
      return;
    }


    update_face_normals();
  //  update_normals();
    update_vertex_normals();
    update_halfedge_normals();

    Mesh::VertexIter vIt(vertices_begin()), vEnd(vertices_end());

    for (; vIt != vEnd; ++vIt)
    {
      Point3f _p = point(vIt);
      Bounds _bounds(_p,_p);
      bounds_.extend(_bounds);
    }

    LOG_MSG << fmt("% % % | % % %")  % bounds_.min().x() % bounds_.min().y() % bounds_.min().z() % 
                                       bounds_.max().x() % bounds_.max().y() % bounds_.max().z();
  }

  void Mesh::draw(const Color4f& _color) const
  {
    typename Mesh::ConstFaceIter    fIt(faces_begin()), 
                                    fEnd(faces_end());
    typename Mesh::ConstFaceVertexIter fvIt;

    glBegin(GL_TRIANGLES);
    for (; fIt!=fEnd; ++fIt)
    {
      glColor4f(_color.r(),_color.g(),_color.b(),_color.a());
      
      /*
      fvIt = cfv_iter(fIt.handle());
      
      Point3f A = point(fvIt); ++fvIt;
      Point3f B = point(fvIt); ++fvIt;
      Point3f C = point(fvIt);
      
      Vec3f _normal = cross(C-A,B-A);
      _normal *= 1.0 / _normal.length(); 
      glNormal3f( COORDS(_normal) );
*/

      fvIt = cfv_iter(fIt.handle());
      
      glNormal3fv( &normal(fvIt)[0] );
      glVertex3fv( &point(fvIt)[0] );
      ++fvIt;
      
      glNormal3fv( &normal(fvIt)[0] );
      glVertex3fv( &point(fvIt)[0] );
      ++fvIt;
      glNormal3fv( &normal(fvIt)[0] );
      glVertex3fv( &point(fvIt)[0] );
    } 
    glEnd();

  }

  void Mesh::optimize(ImplicitSurface& _implicitSurface, unsigned _nVertices, float _quality)
  {
    typename Mesh::ConstFaceIter    _fIt(faces_begin()), 
                                    _fEnd(faces_end());
    typename Mesh::ConstFaceVertexIter _fvIt;
    float _collapse = 0.0;

    FaceCostMap _faces;
    float _costSum = 0.0;
    unsigned _nFaces = 0;

    for (; _fIt!=_fEnd; ++_fIt)
    {
      float _cost = cost(_implicitSurface,_fIt);
      _faces.insert(make_pair<float,Mesh::FaceHandle>(_cost,_fIt.handle()));
      _costSum += _cost;
      _nFaces++;
    }

    float _qualityN = _quality * _nFaces;


    if (_quality >= 0)
    {
      while (_costSum > _qualityN )
      {
        float _cost = _faces.begin()->first;
        Mesh::FaceHandle _fh = _faces.begin()->second;
        Mesh::FaceHalfedgeIter _fheIt = fh_iter(_fh);
        collapse(_fheIt);
        _costSum =- _cost;

        if (_faces.empty()) break;
      }
    }

    if (_nVertices > 0)
    { 
      unsigned i = 0;
      while (i < _nVertices)
      {
        float _cost = _faces.begin()->first;
        Mesh::FaceHandle _fh = _faces.begin()->second;
        Mesh::FaceHalfedgeIter _fheIt = fh_iter(_fh);
        collapse(_fheIt);
        _costSum =- _cost;
        i++;
      }
    }
  }

  float Mesh::cost(const Mesh::VertexHandle& _vHandle)
  {
    typename Mesh::ConstVertexVertexIter _vvBegin = cvv_begin(_vHandle),
                                         _vvEnd = cvv_end(_vHandle),
                                         _vvIt;

    float _cost = 0;
    for (_vvIt = _vvBegin ; _vvIt != _vvEnd ; ++_vvIt )
    {
      Mesh::VertexHandle _vvHandle = _vvIt.handle();
      Vec3f _normal = normal(_vvHandle);
      float _dist = dot(_normal,point(_vHandle) - point(_vvHandle));
      _cost += _dist * _dist;
    }

    return _cost;
  }

  void Mesh::simplify()
  {
    VertexCostMap _vertices;
    typename Mesh::ConstVertexIter  _vIt(vertices_begin()), 
                                    _vEnd(vertices_end());
    typename Mesh::VertexFaceIter vf_it;
    typename Mesh::HalfedgeIter h_it(halfedges_begin());

    for (; _vIt != _vEnd; ++_vIt)
    {
      _vertices.insert(make_pair<float,Mesh::VertexHandle>(cost(_vIt.handle()),_vIt.handle()));
  
    }
    size_t _nVertices = _vertices.size();

    while (_vertices.size() > _nVertices/2)
    {
      float _cost = _vertices.begin()->first;
      Mesh::VertexHandle _vHandle = _vertices.begin()->second; 

      if (_vHandle.is_valid())
      {
      Mesh::HalfedgeHandle _heHandle = halfedge_handle(_vHandle);
        if (_heHandle.is_valid())
        {
          if (face_handle(_heHandle).is_valid() && halfedge_handle(face_handle(_heHandle)).is_valid())

      if (is_collapse_ok(_heHandle));
      {
          collapse(_heHandle); 
      }
}}

      _vertices.erase(_vertices.begin());
      if (_vertices.empty()) break;
    } 
    update_face_normals();
    update_vertex_normals();


  }

  void Mesh::refine()
  {
  }

  float Mesh::cost(const ImplicitSurface& _implicitSurface) const
  {
    typename Mesh::ConstFaceIter    _fIt(faces_begin()), 
                                    _fEnd(faces_end());
    float _cost = 0.0;
    for (; _fIt!=_fEnd;  ++_fIt)
      _cost += cost(_implicitSurface,_fIt.handle());
     
    return _cost;
  }
  
  float Mesh::cost(const ImplicitSurface& _implicitSurface, const Mesh::FaceHandle& _fHandle) const
  {
    typename Mesh::ConstFaceVertexIter _fvIt = cfv_iter(_fHandle);
    Point3f _A = point(_fvIt); ++_fvIt;
    Point3f _B = point(_fvIt); ++_fvIt;
    Point3f _C = point(_fvIt);

    Vec3f _u = _B - _A, _v = _C - _A;
    float _surfaceArea = 0.5 * _u.length() * _v.length();
    unsigned _nSamples = 5;
    
    unsigned _count = 1;
    float _costSum = 0;

    for (unsigned int i = 1; i <= _nSamples; i++)
    {
      float _cost = 0.0;
      _implicitSurface.evaluate(_A,&_cost);
      _costSum =+ _cost;

      float _iStep = float(i) / _nSamples;
      Point3f _stepPointU = _A + _iStep * _u, _stepPointV = _A + _iStep * _v;

      for (unsigned int j = 0; j <= i; j++)
      {
        float _jStep = float(j) / _nSamples;
        Point3f _p = _stepPointU + (_stepPointV - _stepPointU) * _jStep;
        _implicitSurface.evaluate(_p,&_cost);
        _costSum += _cost;
        _count++;
      }
    }

    _costSum /= _count;
    return _costSum * _surfaceArea;
  }
}
