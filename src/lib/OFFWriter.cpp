#include "cg2/OFFWriter.hpp"
#include <boost/foreach.hpp>

#include <fstream>

using namespace std;

namespace cg2 
{
  void OFFWriter::write(std::string filename, const Vertices* vertices, const Polygons* polygons) const
  {

    unsigned V = vertices ? vertices->size() : 0,
             F = polygons ? polygons->size() : 0,
             E = 0;

    ofstream os(filename.c_str());

    os << "OFF" << endl;
    os << V << " " << F << " " << E << endl;

    if (vertices)
    {
      BOOST_FOREACH( const Vertex& vertex, *vertices )
        os << vertex.v.x() << " " << vertex.v.y() << " " << vertex.v.z() << endl;
    }

    if (polygons)
    {
      BOOST_FOREACH( const Polygon& polygon, *polygons )
      {
         os << polygon.size();
         BOOST_FOREACH( Vertex* vertex, polygon )
         {
           long long vertexIdx = ((long long)vertex - (long long)vertices) / sizeof(Vertex);
           os << vertexIdx;
         }
      }
    }
  }

}
