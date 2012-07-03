#include "cg2/Mesh.hpp"

#include <boost/foreach.hpp>
#include "cg2/OFFReader.hpp"

#include <tbd/log.h>

#include <GL/glu.h>

using namespace std;

namespace cg2
{
  void Mesh::draw(const Color4f& _color) const
  {
 /*   BOOST_FOREACH ( const VertexTriangle& tri, objs_ )
    {
   //   LOG_MSG;
      tri.draw(_color);
    }*/
  }

  void Mesh::optimize(ImplicitSurface& _implicitSurface, unsigned _nVertices, float _quality)
  {
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
    }
  }


  void Mesh::simplify()
  {
  }

  void Mesh::refine()
  {
  }

  float cost(const ImplicitSurface& _implicitSurface) const
  {
    float _cost = 0.0;
    BOOST_FOREACH ( const Triangle& _triangle, this->objs() )
      _cost += cost(_implicitSurface,_triangle);
  
    return _cost;
  }
  
  float Mesh::cost(const ImplicitSurface& _implicitSurface, const Triangle& _triangle) const
  {
    float _surfaceArea = _triangle.surfaceArea();
    unsigned _nSamples = 5;
    
    unsigned _count = 0;
    float _costSum = 0;

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

    _costSum /= _count;
    return _costSum * _surfaceArea;
  }
}
