#include "cg2/TriangleMesh.hpp"
#include "cg2/OFFReader.hpp"

#include <boost/foreach.hpp>
#include "tbd/log.h"

using namespace std;

namespace cg2
{
  TriangleKDTree::TriangleKDTree()
  {
  }

  void TriangleKDTree::divideNode(Node* node, BoundingBox& box, int depth)
  {
    if (depth > 15 || node->objs.size() < 10)
    {   // We have a leaf node!
      node->left = NULL; node->right = NULL;
      return;
    }
    node->left = new Node;
    node->right = new Node;
    node->axis = box.dominantAxis();

    // TODO: Surface Area Heuristic here!
    node->splitPos = 0.5*(box.min.cell[node->axis] + box.max.cell[node->axis]);
    BoundingBox boxLeft, boxRight;
    box.split(node->splitPos,node->axis,boxLeft,boxRight);
    
    BOOST_FOREACH( Triangle* tri, node->objs )
    {
      int result = tri->splitPlaneIntersect(node->splitPos,node->axis);
      if (result & 1) node->left->objs.push_back(tri);
      if (result & 2) node->right->objs.push_back(tri);
    }
    node->objs.clear();
    divideNode(node->left,boxLeft,depth+1);
    divideNode(node->right,boxRight,depth+1);
  }

  void TriangleMesh::read(string filename)
  {
    PolygonMesh::read(filename);
    calcTriangles();
    kdTree.build(triangles(),boundingBox());
  }

	void TriangleMesh::calcTriangles()
	{
    triangles_.reserve(polygons().size());
    BOOST_FOREACH ( Polygon& p, polygons() )
    {
			if (p.size()==3)
			{
				Triangle tri(p[0],p[1],p[2]);
				tri.shader(shader());
				triangles_.push_back(tri);
			}
    }
	}


  float TriangleKDTree::recKDTreeTraverse(Ray& ray, Node* node, float tnear, float tfar, bool& found)
  {
    if (node->isLeaf())
    {
      BOOST_FOREACH( Triangle* tri, node->objs )
        if (tri != ray.obj) found |= (tri->intersect(ray));
      return ray.tmax;
    }

    int k = node->axis;
    float d = (node->splitPos - ray.org[k]) / ray.dir[k];

    KDNode<Triangle>* front = node->left;
    KDNode<Triangle>* back  = node->right;
    if (ray.dir[k] < 0) swap(front,back); 

    if (d <= tnear)
    {
      recKDTreeTraverse(ray,back,tnear,tfar,found);
    } else
      if (d >= tfar)
      {
        recKDTreeTraverse(ray,front,tnear,tfar,found);
      } else
      {
        float t_hit = recKDTreeTraverse(ray,front,tnear,d,found);
        if (t_hit <= d) return t_hit;
        return recKDTreeTraverse(ray,back,d,tfar,found);
      }
    return INF;
  }

  bool TriangleMesh::intersect(Ray& ray)
  {
    bool found = false;
    kdTree.recKDTreeTraverse(ray,kdTree.root,ray.tmin,ray.tmax,found);
    return found;
  }


}
