#include "cg2/PolygonMesh.hpp"
#include "cg2/OFFReader.hpp"

#include <boost/foreach.hpp>

#include <GL/gl.h>

namespace cg2
{
  void PolygonMesh::read(string filename)
  {
    OFFReader off;
    off.read(filename,&vertices,&polygons_);
    calcBoundingBox();
    calcNormals();
  }

  void PolygonMesh::calcNormals()
  {
    BOOST_FOREACH(Polygon& polygon, polygons_)
    {
      Vec3f n = polygon.normal();
      BOOST_FOREACH(Vertex* vertex, polygon.vertices)
      vertex->n += n;
    }

    BOOST_FOREACH(Vertex& vertex, vertices)
    vertex.n.normalize();
  }

  void PolygonMesh::displayNormals()
  {
    BOOST_FOREACH(Vertex& vertex, vertices)
    {
      Vec3f n = boundingBox_.size().length()*0.02f*vertex.n;

      glPushMatrix();
      glTranslatef(vertex.v.x,vertex.v.y,vertex.v.z);
      glBegin(GL_LINES);
      glColor3f(0.0, 1.0, 0.0);
      glVertex3f(0.0 , 0.0 , 0.0);
      glVertex3f(n.x,n.y,n.z);
      glColor3f(0.0, 0.0, 0.0);
      glEnd();

      glPopMatrix();
    }
  }

  void PolygonMesh::draw(Color color)
  {
    BOOST_FOREACH(Polygon& polygon, polygons_)
    {
      glBegin(GL_TRIANGLES);
      BOOST_FOREACH(Vertex* vertex, polygon.vertices)
      {
        glColor3f(color.x,color.y,color.z);
        glNormal3f(vertex->n.x,vertex->n.y,vertex->n.z);
        glVertex3f(vertex->v.x,vertex->v.y,vertex->v.z);
      }
      glEnd();
    }
  }


}
