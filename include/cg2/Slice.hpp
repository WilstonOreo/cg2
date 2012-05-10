#pragma once

#include <vector>

namespace cg2
{
  using std::vector;

  struct Point2f
  {
    float x,y;
  };

  typedef vector<Point2f> Polyline;
  typedef vector<Polyline> Polylines;
  typedef vector<Polyline*> PolylineList;

  class Track : public Polyline
  {
  };

  struct Slice 
  {
    Polylines border;
    Polylines innerSupport, outerSupport;	  

    float z;
  };

  typedef vector<Slice> SliceList;
  typedef vector<Slice*> Slices;
}
