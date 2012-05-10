#include "cg2/Mesh.hpp"

#include <boost/foreach.hpp>

#include <tbd/log.h>

namespace cg2
{
  void Mesh::calcBoundingBox()
  {
    boundingBox().min.set(INF,INF,INF);
    boundingBox().max.set(-INF,-INF,-INF);

    BOOST_FOREACH( Vertex& vertex, *this )
    {
      Point3f v = vertex.v;
      for (int j = 0; j < 3; j++)
      {
        if (v.cell[j] > boundingBox().max.cell[j]) boundingBox().max.cell[j] = v.cell[j];
        if (v.cell[j] < boundingBox().min.cell[j]) boundingBox().min.cell[j] = v.cell[j];
      }
    }
  }



/*
  void Mesh::divideNode(KDNode* node, BoundingBox& box, int depth)
  {
    if (depth > 15 || node->tris.size() < 10)
    {   // We have a leaf node!
      node->left = NULL; node->right = NULL;
      return;
    }
    node->left = new KDNode();
    node->right = new KDNode();
    node->axis = box.dominantAxis();

    // TODO: Surface Area Heuristic here!
    node->splitPos = 0.5*(box.min.cell[node->axis] + box.max.cell[node->axis]);
    BoundingBox boxLeft, boxRight;
    box.split(node->splitPos,node->axis,boxLeft,boxRight);
    for (size_t i = 0; i < node->tris.size(); i++) 	
    {
      Triangle* tri = node->tris.at(i);
      int result = tri->splitPlaneIntersect(node->splitPos,node->axis);
      if (result & 1) node->left->tris.push_back(tri);
      if (result & 2) node->right->tris.push_back(tri);
    }
    node->tris.clear();
    divideNode(node->left,boxLeft,depth+1);
    divideNode(node->right,boxRight,depth+1);
  }

  void Mesh::buildKDTree()
  {
    cout << "Building KD Tree..." << endl;
    kdtree = new KDNode();
    kdtree->tris.resize(triangles.size());
    for (size_t i = 0; i < triangles.size(); i++)
      kdtree->tris[i] = &triangles[i];
    divideNode(kdtree,box,0);
  }

  float Mesh::recKDTreeTraverse(Ray& ray, KDNode* node, float tnear, float tfar, bool& found)
  {
    if (node->isLeaf())
    {
      for (size_t i = 0; i < node->tris.size(); i++)
        if (node->tris.at(i)->intersect(ray))
          found = true;
      return ray.tmax;
    }

    int k = node->axis;
    float d = (node->splitPos - ray.org.cell[k]) / ray.dir.cell[k];


    KDNode* front = node->left;
    KDNode* back  = node->right;
    if (ray.dir.cell[k] < 0) swap(front,back); 

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
  }

*/
}
