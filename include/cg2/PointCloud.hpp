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
			set<Vertex const *> vertexSet();
			float maxDist();

			TBD_DECLARE_PROPERTY(int,k);
			TBD_DECLARE_PROPERTY(float,radius);
			TBD_DECLARE_PROPERTY_REF(Point3f,center);
	};

	class PointKDTree : public KDTree<Vertex> {
		public:
			PointKDTree() : drawDepth_(10) {}
			void collect(KDNode<Vertex> const * node, BoundingBox const & box, PointSet & pointSet) const;

			TBD_DECLARE_PROPERTY(unsigned,drawDepth);
		private:
			void divideNode(KDNode<Vertex> * node, BoundingBox & box, int depth);

			float nodeDistance(Point3f const & p, BoundingBox const & box) const;
	};


	class PointCloud : public Mesh {
		public:
			PointCloud();

			void read(string const & filename);
			void write(string const & filename) const;

			void draw(Color const & color = Color()) const;

			Vec3f normal(const Ray & ray) const {
				return Vec3f();
			}
			TexCoords texCoords(const Ray & ray) const {
				return TexCoords();
			}

			bool intersect(Ray & ray) const {
				return false;
			}


			void update();
			void collectKNearest(Point3f const & p, int k);
			void collectInRadius(Point3f const & p, float radius);

			set<Vertex const *> selection;

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
