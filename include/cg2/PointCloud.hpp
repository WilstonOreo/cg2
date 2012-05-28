#pragma once

#include "cg2/Mesh.hpp"
#include "cg2/KDTree.hpp"

#include <set>

namespace cg2 {
	struct SelectedPoint {
		SelectedPoint(float _dist, Vertex * _v) : dist(_dist), v(_v) {}
		float dist;
		Vertex * v;
	};

	struct PointCompare {
		bool operator()(const SelectedPoint & a, const SelectedPoint & b) {
			return a.dist < b.dist;
		}
	};

	class PointSet : public std::set<SelectedPoint,PointCompare> {
		public:
			PointSet(Point3f _center, float _radius = 0.0, int _k = 0);

			bool insert(Vertex * v);
			VertexSet vertexSet();
			float maxDist();

			TBD_DECLARE_PROPERTY(int,k);
			TBD_DECLARE_PROPERTY(float,radius);
			TBD_DECLARE_PROPERTY_REF(Point3f,center);
	};

	class PointKDTree : public KDTree<Vertex> {
		public:
			PointKDTree() : drawDepth_(10) {}
			void collect(KDNode<Vertex> * node, BoundingBox & box, PointSet & pointSet);

			TBD_DECLARE_PROPERTY(unsigned,drawDepth);
		private:
			void divideNode(KDNode<Vertex> * node, BoundingBox & box, int depth);

			float nodeDistance(Point3f & p, BoundingBox & box);
	};


	class PointCloud : public Mesh {
		public:
			PointCloud();

			void read(string filename);
			void write(string filename);

			void draw(Color color = Color());

			Vec3f normal(const Ray & ray) {
				return Vec3f();
			}
			TexCoords texCoords(const Ray & ray) {
				return TexCoords();
			}

			bool intersect(Ray & ray) {
				return false;
			}


			void update();
			void collectKNearest(Point3f & p, int k);
			void collectInRadius(Point3f & p, float radius);

			VertexSet selection;

			TBD_DECLARE_PROPERTY(bool,drawKDTree);
			TBD_DECLARE_PROPERTY_REF(Color,kdTreeColor);
			TBD_DECLARE_PROPERTY(bool,drawBoundingBox);
			TBD_DECLARE_PROPERTY_REF(Color,boundingBoxColor);

			TBD_DECLARE_PROPERTY(bool,highlightSelection);
			TBD_DECLARE_PROPERTY(Color,selectionColor);
		private:
			PointKDTree kdTree;
	};
}
