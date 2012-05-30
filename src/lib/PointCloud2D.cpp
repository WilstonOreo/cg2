#include "include/cg2/PointCloud2D.hpp"
#include "tbd/log.h"
#include <cassert>
#include <boost/foreach.hpp>
#include "cg2/Median.hpp"

namespace cg2 
{
	PointSet2D::PointSet2D(Point3f _center, float _radius, int _k) : PointSet(_center,_radius,_k) 
  {
	}

	bool PointSet2D::insert(Vertex * v) {

    Vec3f distVec = v->v-center(); distVec.z = 0.0;
    float dist = distVec.length();
		if (radius() > 0.0f && dist > radius()) return false;

		if (k() && int(size()) >= k())
			if ((--end())->dist < dist) return false;

		SelectedPoint p(dist,v);
		std::set<SelectedPoint,PointCompare>::insert(p);

		PointSet::iterator it = end();
		it--;
		if (k() && int(size()) > k() && !empty()) {
			erase(it);
		}
		return true;
	}


	static bool compareX(const Vertex * a, const Vertex * b) {
		return a->v.x < b->v.x;
	}
	static bool compareY(const Vertex * a, const Vertex * b) {
		return a->v.y < b->v.y;
	}

	void PointKDTree2D::collect(KDNode<Vertex> const * node, BoundingBox const & box, PointSet2D & pointSet) const {
		if (!node) {
			return;
		}

		if (node->isLeaf()) {
			BOOST_FOREACH(Vertex* vertex, node->objs)
			pointSet.insert(vertex);
			return;
		}

		BoundingBox boxLeft, boxRight;
		box.split(node->splitPos,node->axis,boxLeft,boxRight);

		if (nodeDistance(pointSet.center(),boxLeft) < pointSet.maxDist()) {
			collect(node->left,boxLeft,pointSet);
		}
		if (nodeDistance(pointSet.center(),boxRight) < pointSet.maxDist()) {
			collect(node->right,boxRight,pointSet);
		}
	}
	void PointKDTree2D::divideNode(KDNode<Vertex> * node, BoundingBox & box, int depth) {
		assert(node);
		LOG_MSG_(2) << fmt("Depth: %, objs: %") % depth % node->objs.size();

		if (depth > 20 || node->objs.size() < 4) {
			// We have a leaf node!
			node->left = NULL;
			node->right = NULL;
			return;
		}
		node->left = new KDNode<Vertex>();
		node->right = new KDNode<Vertex>();
    node->axis = (box.size().x > box.size().y) ? X : Y;

		switch (node->axis) 
    {
			case X:
				node->splitPos = median(node->objs,compareX)->v.x;
				break;
			case Y:
				node->splitPos = median(node->objs,compareY)->v.y;
				break;
			default:
				return;
		}
		BoundingBox boxLeft, boxRight;
		box.split(node->splitPos,node->axis,boxLeft,boxRight);

		BOOST_FOREACH(Vertex* vertex, node->objs) {
			KDNode<Vertex> * subNode = (vertex->v[node->axis] < node->splitPos) ? node->left : node->right;
			subNode->objs.push_back(vertex);
		}
		node->objs.clear();
		divideNode(node->left,boxLeft,depth+1);
		divideNode(node->right,boxRight,depth+1);
	}

	float PointKDTree2D::nodeDistance(Point3f const & p, BoundingBox const & box) const {
		if (box.pointInBox(p)) return 0.0;

		float minDist = INF;
		FOREACH_AXIS 
    {
			if (axis == Z) continue;
			minDist = std::min(std::abs(p[axis] - box.min[axis]),std::abs(box.max[axis] - p[axis]));
		}

		return minDist;
	}


	void PointCloud2D::update() {
		calcBoundingBox();
		kdTree.build(vertices,boundingBox());
	}

	set<Vertex const *> PointCloud2D::collectKNearest(Point3f const & p, int k) const {
		PointSet2D pointSet(p,0.0,k);
		kdTree.collect(kdTree.root,boundingBox(),pointSet);
		return pointSet.vertexSet();
	}

	set<Vertex const *> PointCloud2D::collectInRadius(Point3f const & p, float radius) const {
		PointSet2D pointSet(p,radius);
		kdTree.collect(kdTree.root,boundingBox(),pointSet);
		return pointSet.vertexSet();
	}

  
}
