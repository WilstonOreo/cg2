#include "cg2/PointCloud.hpp"
#include "cg2/Median.hpp"

#include <boost/foreach.hpp>

#include <cassert>

#include "cg2/OFFReader.hpp"
#include "cg2/OFFWriter.hpp"

#include <GL/glu.h>

#include "tbd/log.h"

using namespace std;

namespace cg2
{
  typedef pair<double,Vertex *> entry;

  PointSet::PointSet(Point3f _center, float _radius, int _k) : k_(_k), radius_(_radius), center_(_center)
  {

  }

  bool PointSet::insert(Vertex * v)
  {
    Vec3f distVec = v->v-center();
    float dist = distVec.length();
    if (dist > radius()) return false;

    if (int(points.size()) >= k())
      if (!points.empty() && (--points.end())->first < dist) return false;

    points.insert(make_pair(dist, v));

    if (points.empty())
      return true;

    multimap<double, Vertex *>::iterator it = points.end();
    it--;
    if (int(points.size()) > k())
    {
      points.erase(it);
    }
    return true;
  }


  set<const Vertex*> PointSet::vertexSet()
  {
    set<const Vertex*> result;
    BOOST_FOREACH( const entry& p, points )
      result.insert(p.second);
    return result;
  }

  float PointSet::maxDist()
  {
    if (!k_ && radius_ > 0.0f)
    {
      return radius_;
    }

    if (k_)
    {
      if (k_ < int(points.size()) && radius_ >  0.0f)
      {
        return radius_;
      }
      if (k_ >= int(points.size()) && !points.empty())
      {
        multimap<double,Vertex *>::iterator it = points.end();
        it--;
        return it->first;
      }
    }
    return INF;
  }
 
  static bool compareX(const Vertex* a, const Vertex* b) { return a->v.x() < b->v.x(); }
  static bool compareY(const Vertex* a, const Vertex* b) { return a->v.y() < b->v.y(); }
  static bool compareZ(const Vertex* a, const Vertex* b) { return a->v.z() < b->v.z(); }

  void PointCloud::collect(KDNode<Vertex>* node, const BoundingBox& box, PointSet& pointSet) const
  {
    if (!node) return;

    if (node->isLeaf())
    {
      BOOST_FOREACH( Vertex* vertex, node->objs )
        pointSet.insert(vertex);
      return;
    }
    
    BoundingBox boxLeft, boxRight;
    box.split(node->splitPos,node->axis,boxLeft,boxRight);
    
    if (nodeDistance(pointSet.center(),boxLeft) < pointSet.maxDist()) 
      collect(node->left,boxLeft,pointSet);
    if (nodeDistance(pointSet.center(),boxRight) < pointSet.maxDist())
      collect(node->right,boxRight,pointSet);
  }

  void PointCloud::divideNode(KDNode<Vertex>* node, const BoundingBox& box, int depth)
  {
    assert(node);
    LOG_MSG_(2) << fmt("Depth: %, objs: %") % depth % node->objs.size();

    if (depth > 20 || node->objs.size() < 4)
    {   // We have a leaf node!
      node->left = NULL; node->right = NULL;
      return;
    }
    node->left = new KDNode<Vertex>();
    node->right = new KDNode<Vertex>();
    node->axis = box.dominantAxis();

    switch (node->axis)
    {
      case X: node->splitPos = median(node->objs,compareX)->v.x(); break;
      case Y: node->splitPos = median(node->objs,compareY)->v.y(); break;
      case Z: node->splitPos = median(node->objs,compareZ)->v.z(); break;
      default: return;
    }
    BoundingBox boxLeft, boxRight;
    box.split(node->splitPos,node->axis,boxLeft,boxRight);
    
    BOOST_FOREACH( Vertex* vertex, node->objs )
    {
      KDNode<Vertex>* subNode = (vertex->v[node->axis] < node->splitPos) ? node->left : node->right;
      subNode->objs.push_back(vertex);
    }
    node->objs.clear();
    divideNode(node->left,boxLeft,depth+1);
    divideNode(node->right,boxRight,depth+1);
  }

  float PointCloud::nodeDistance(const Point3f& p, const BoundingBox& box) const
  {
    if (box.pointInBox(p)) return 0.0;

    float minDist = INF;
    FOREACH_AXIS 
    {
      minDist = std::min(std::abs(p[axis] - box.min()[axis]),std::abs(box.max()[axis] - p[axis]));
    }

  //  LOG_MSG << minDist;
    return minDist;
  }


  PointCloud::PointCloud() : drawKDTree_(false), 
                             //kdTreeColor_(Color(0.0,0.2,0.4)), 
                             drawBoundingBox_(false)
                             //boundingBoxColor_(Color(0.0,1.0,0.0)),
                             //selectionColor_(Color(1.0,1.0,1.0))
  {

  }

  void PointCloud::read(const string& filename)
  {
    OFFReader off;
 //   off.read(filename,&this->objs_,NULL);
    update();
  }

  void PointCloud::write(const string& filename) const
  {
    OFFWriter off;
    off.write(filename,&this->objs_,NULL);
  }

  void PointCloud::update()
  {
    calcBoundingBox();
    build(objs_,boundingBox_);
  }

  void PointCloud::draw(const Color4f& _color) const
  {
    if (drawBoundingBox_ && !drawKDTree_) boundingBox_.draw(boundingBoxColor());

    glBegin(GL_POINTS);
    BOOST_FOREACH( const Vertex& vertex, objs_ )
    {
      if (selection.count(&vertex))
        glColor4f(selectionColor().r(),selectionColor().g(),selectionColor().b(),selectionColor().a());
      else
        glColor4f(_color.r(),_color.g(),_color.b(),_color.a());
      glVertex3f(COORDS(vertex.v));
    }
    glEnd(); 
  }


  std::set<const Vertex*> PointCloud::collectKNearest(const Point3f& p, int k) const
  {
    PointSet pointSet(p,0.0,k);
    collect(root_,boundingBox_,pointSet);
    return pointSet.vertexSet();
  }

  std::set<const Vertex*> PointCloud::collectInRadius(const Point3f& p, float radius) const
  {
    PointSet pointSet(p,radius); 
    collect(root_,boundingBox_,pointSet);
    return pointSet.vertexSet();
  }

  bool PointCloud::isNearest(const Vertex& _v, const Point3f& _p) const
  {
    float radius = (_v.v - _p).length();
    return collectInRadius(_p,radius).size() <= 1;
  }

}
