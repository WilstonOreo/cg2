#include "include/cg2/PointCloud2D.hpp"
#include "tbd/log.h"
#include <cassert>
#include <boost/foreach.hpp>
#include "cg2/Median.hpp"
#include <GL/glu.h>

namespace cg2 {
	PointSet2D::PointSet2D(Point3f _center, float _radius, int _k) : PointSet(_center,_radius,_k) {
	}

	bool PointSet2D::insert(Vertex * v) {
		Vec3f distVec = v->v - center();
		distVec.z = 0.0;
		float dist = distVec.length();
		if (dist > radius()) {
			return false;
		}

		if (int(points.size()) >= k())
			if (!points.empty() && (--points.end())->first < dist) {
				return false;
			}

		points.insert(make_pair(dist,v));

		if (points.empty()) {
			return true;
		}

		map<double, Vertex *>::iterator it = points.end();
		it--;
		if (int(points.size()) > k()) {
			points.erase(it);
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

		switch (node->axis) {
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
		if (box.pointInBox(p)) {
			return 0.0;
		}

		float minDist = INF;
		FOREACH_AXIS {
			if (axis == Z) {
				continue;
			}
			minDist = std::min(std::abs(p[axis] - box.min[axis]),std::abs(box.max[axis] - p[axis]));
		}

		return minDist;
	}



	static double wendland(double d, double h) {
		/*x *= h;
		const double Q_PI = 3.14159265358979323846;
		return (1/sqrt(2*Q_PI))*exp(-0.5*d*d); // gauss, not wendland*/
		return std::pow(1-d/h, 4)*(4*d/h+1);
	}

	void PointCloud2D::generateGrid(PointCloud const & in) {
		BoundingBox box = in.boundingBox();
		float const xdist = box.size().x / (width_-1), ydist = box.size().y / (height_-1);
		float const xmin = box.min.x, ymin = box.min.y;
		float const radius = max(sqrt(xdist*xdist + ydist*ydist)*2, 0.08f);
		vertices.clear();
		vertices.reserve(width_*height_);

		for (size_t xp = 0; xp < width_; xp++) {
			float x = xmin + xp*xdist;
			for (size_t yp = 0; yp < height_; yp++) {
				float y = ymin + yp*ydist;
				Vertex currentXY(Point3f(x, y, 0));
				int nPoints = 0;
				double sumWeight = 0;
				double currentZ = 0;
				set<Vertex const *> kNearest = in.collectInRadius(currentXY.v,radius);

				BOOST_FOREACH(Vertex const * v, kNearest) {
					Point3f const & p = v->v;
					Point3f pFlat(p);
					pFlat.z = 0;
					double distance = (currentXY.v-pFlat).length(); // Distanz zwischen P und (x,y,0)
					double weight = wendland(distance, radius);

					sumWeight += weight;
					currentZ += weight * p.z;
					nPoints++;
				}
				if (nPoints && sumWeight) {
					currentZ /= sumWeight;
				}
				currentXY.v.z = currentZ;
				vertices.push_back(currentXY);
			}
		}

		// Calculate Normals
		for (size_t xp = 0; xp < width_; xp++) {
			for (size_t yp = 0; yp < height_; yp++) {
				Vertex & v = getVertex(xp, yp);
				Point3f v00 = v.v;
				Point3f v01 = getVertex(xp  , yp+1).v;
				Point3f v10 = getVertex(xp+1, yp  ).v;
				Point3f v0n = getVertex(xp  , yp-1).v;
				Point3f vn0 = getVertex(xp-1, yp  ).v;
				v.n = (
					(v10 - v00).cross(v01-v00).normalized() +
					(v00 - vn0).cross(v00-v0n).normalized()
				).normalized();
			}
		}

		update();
	}

	template<class T> mathvector<T> casteljau(mathvector<T> const & in, double t) {
		mathvector<T> result(in.size()-1);

		for (size_t i = 0; i < result.size(); ++i) {
			result[i] = in[i]+(in[i+1]-in[i])*t;
		}

		return result;
	}

	template<class T> T casteljau_value(mathvector<T> const & in, double t) {
		mathvector<T> out(in);

		while (out.size() > 1) {
			mathvector<T> tmp = casteljau(out, t);
			tmp.swap(out);
		}

		return out.front();
	}

	template<class T> mathvector<T> casteljau_diff(mathvector<T> const & in, double t) {
		mathvector<T> out(in);

		while (out.size() > 2) {
			mathvector<T> tmp = casteljau(out, t);
			tmp.swap(out);
		}

		return out;
	}

	void PointCloud2D::generateCasteljauGrid(PointCloud2D const & in) {
		size_t inWidth = in.width();
		size_t inHeight = in.height();
		mathvector< mathvector<Vec3f> > grid(inWidth, mathvector<Vec3f>(inHeight));

		for (size_t xp = 0; xp < inWidth; xp++) {
			for (size_t yp = 0; yp < inHeight; yp++) {
				grid[xp][yp] = in.getVertex(xp, yp).v.vec3f();
			}
		}

		BoundingBox box = in.boundingBox();
		float const xdist = 1.0 / (width_-1), ydist = 1.0 / (height_-1);
		float const xmin = 0, ymin = 0;

		vertices.clear();
		for (size_t xp = 0; xp < width_; xp++) {
			float x = xmin + xp*xdist;
			mathvector< mathvector<Vec3f> > diffLines = casteljau_diff(grid, x);
			mathvector<Vec3f> frontLine = diffLines.front();
			mathvector<Vec3f> backLine = diffLines.back();

			mathvector<Vec3f> valueLine = casteljau_value(diffLines, x);
			for (size_t yp = 0; yp < height_; yp++) {
				float y = ymin + yp*ydist;
				if (true) {
					mathvector<mathvector<Vec3f> > diffPoints;
					diffPoints.push_back(casteljau_diff(frontLine,y));
					diffPoints.push_back(casteljau_diff(backLine,y));

#define dp(a,b) diffPoints[a-xp][b-yp]
					const Vec3f & v00 = dp(xp  , yp  );
					const Vec3f & v01 = dp(xp  , yp+1);
					const Vec3f & v10 = dp(xp+1, yp  );

					Vec3f normal = (v10 - v00).cross(v01-v00).normalized();

					Vec3f valuePoint = casteljau_value(valueLine, y);
					Vertex vertex(Point3f(valuePoint.x, valuePoint.y, valuePoint.z), normal);
					vertices.push_back(vertex);
				}
				else {
					vertices.push_back(in.getVertex(xp, yp));
				}
			}
		}
	}

	void PointCloud2D::drawSurface(Color const & color) {
		glBegin(GL_QUADS);
		for (unsigned xp = 0; xp+1 < width_; xp++) {
			for (unsigned yp = 0; yp+1 < height_; yp++) {
				const Vertex & v00 = getVertex(xp  , yp  );
				const Vertex & v01 = getVertex(xp  , yp+1);
				const Vertex & v10 = getVertex(xp+1, yp  );
				const Vertex & v11 = getVertex(xp+1, yp+1);

				//glColor3f(color.x,color.y,color.z);
				glNormal3f(v00.n.x,v00.n.y,v00.n.z);
				glVertex3f(v00.v.x,v00.v.y,v00.v.z);
				glNormal3f(v10.n.x,v10.n.y,v10.n.z);
				glVertex3f(v10.v.x,v10.v.y,v10.v.z);
				glNormal3f(v11.n.x,v11.n.y,v11.n.z);
				glVertex3f(v11.v.x,v11.v.y,v11.v.z);
				glNormal3f(v01.n.x,v01.n.y,v01.n.z);
				glVertex3f(v01.v.x,v01.v.y,v01.v.z);
			}
		}
		glEnd();
	}


	void PointCloud2D::update() {
		calcBoundingBox();
		kdTree.build(vertices,boundingBox());
	}

	set<Vertex const *> PointCloud2D::collectKNearest(Point3f const & p, int k) const {
		PointSet2D pointSet(p,std::numeric_limits<float>::max(),k);
		kdTree.collect(kdTree.root,boundingBox(),pointSet);
		return pointSet.vertexSet();
	}

	set<Vertex const *> PointCloud2D::collectInRadius(Point3f const & p, float radius) const {
		PointSet2D pointSet(p,radius);
		kdTree.collect(kdTree.root,boundingBox(),pointSet);
		return pointSet.vertexSet();
	}
}
