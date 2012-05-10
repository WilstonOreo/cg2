#include "cg2/PolygonMesh.hpp"

#include <boost/foreach.hpp>

#ifdef __APPLE__ 
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

namespace cg2
{
  void PolygonMesh::read(string filename)
  {
    calcBoundingBox();
    calcNormals();
  }

  void PolygonMesh::calcNormals()
  {
    BOOST_FOREACH( Polygon& polygon, polygons_ )
    {
      Vec3f n = polygon.normal();
      BOOST_FOREACH ( Vertex* vertex, polygon )
        vertex->n += n;
    }
   
    BOOST_FOREACH( Vertex& vertex, *this )
      vertex.n.normalize();
  }

  void PolygonMesh::displayNormals()
  {
    BOOST_FOREACH( Vertex& vertex, *this )
    {
      Vec3f n = boundingBox().size().length()*0.02f*vertex.n; 

      glPushMatrix();
      glTranslatef(vertex.v.x,vertex.v.y,vertex.v.z);
      glBegin(GL_LINES);
      glColor3f( 0.0, 1.0, 0.0);
      glVertex3f( 0.0 , 0.0 , 0.0 );
      glVertex3f(n.x,n.y,n.z);
      glColor3f( 0.0, 0.0, 0.0);
      glEnd();

      glPopMatrix();
    }
  }

  void PolygonMesh::draw(Color color)
  {
    BOOST_FOREACH( Polygon& polygon, polygons_ ) 
    {
      glBegin(GL_TRIANGLES);
      BOOST_FOREACH( Vertex* vertex, polygon )
      {
        glColor3f(color.x,color.y,color.z);
        glNormal3f(vertex->n.x,vertex->n.y,vertex->n.z);		
        glVertex3f(vertex->v.x,vertex->v.y,vertex->v.z);
      }
      glEnd();
    }
  }


}
