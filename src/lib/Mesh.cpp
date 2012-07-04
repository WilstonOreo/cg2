#include "cg2/Mesh.hpp"

#include <boost/foreach.hpp>
#include "cg2/OFFReader.hpp"

#include <fstream>
#include <boost/algorithm/string/trim.hpp>
#include <boost/algorithm/string/split.hpp>


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
      fvIt = cfv_iter(fIt.handle());
      
      Point3f A = point(fvIt); ++fvIt;
      Point3f B = point(fvIt); ++fvIt;
      Point3f C = point(fvIt);
      
      Vec3f _normal = cross(C-A,B-A);
      _normal *= 1.0 / _normal.length(); 

 //     LOG_MSG << fmt("% % %") % _normal.x() % _normal.y() % _normal.z();


   // glNormal3f(1.0,1.0,1.0);
     glNormal3f( COORDS(_normal) );

      fvIt = cfv_iter(fIt.handle());
      
      glVertex3fv( &point(fvIt)[0] );
      ++fvIt;
      glVertex3fv( &point(fvIt)[0] );
      ++fvIt;
      glVertex3fv( &point(fvIt)[0] );
    } 
    glEnd();

  }

  void Mesh::optimize(ImplicitSurface& _implicitSurface, unsigned _nVertices, float _quality)
  {
    /*
    const _collapse = 0.0;

    multimap<float,FaceHandle*> _faces;
    float _costSum = 0.0;

    unsigned _nFaces = 0;

    BOOST_FOREACH ( const Triangle& _triangle, this->objs() )
    {
      float _cost = cost(_implicitSurface,_triangle_triangle);
      _faces.insert(make_pair<float,FaceHandle*>(cost));
      _costSum += _cost;
      _nFaces++;
    }

    float _qualityN = _quality * _nFaces;

    if (_quality >= 0)
    {
      while (_costSum > _qualityN )
      {
        float _cost = _faces.begin()->first;
        FaceHandle* _faceHandle = faces.begin()->second;
   //     remove_face(_faceHandle);
        _faces.erase(_faces.begin());
        _costSum =- _cost;
      }
    }

    if (_quality >= 0)
    { 
      unsigned i = 0;
      while (i < _nVertices)
      {
        float _cost = _faces.begin()->first;
        FaceHandle* _faceHandle = faces.begin()->second;
   //     remove_face(_faceHandle);
        _faces.erase(_faces.begin());
        _costSum =- _cost;
        i++;
      }
    }*/
  }


  void Mesh::simplify()
  {
  }

  void Mesh::refine()
  {
  }

  float Mesh::cost(const ImplicitSurface& _implicitSurface) const
  {
    
    float _cost = 0.0;
    /*BOOST_FOREACH ( const Triangle& _triangle, this->objs() )
      _cost += cost(_implicitSurface,_triangle);
  */
    return _cost;
  }
  
  float Mesh::cost(const ImplicitSurface& _implicitSurface, const Triangle& _triangle) const
  {
    float _surfaceArea = 0; //_triangle.surfaceArea();
    unsigned _nSamples = 5;
    
    unsigned _count = 0;
    float _costSum = 0;
/*
    for (int i = 0; i <= _nSamples; i++)
      for (int j = 0; j <= i; j++)
      {
        float _cost = 0.0;
        Point3f _p = _triangle.p0() + (float(i) / _nSamples) * (_triangle.p1() - _triangle.p0()) +
                    (float(i) / _nSamples) * (_triangle.p2() - _triangle.p0());
        _implicitSurface.evaluate(_p,&_cost);
        _costSum += _cost;
        count++;
      }
*/
    _costSum /= _count;
    return _costSum * _surfaceArea;
  }
}
