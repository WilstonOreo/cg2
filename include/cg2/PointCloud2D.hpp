#ifndef POINTCLOUD2D_H
#define POINTCLOUD2D_H

#include "PointCloud.hpp"

namespace cg2 {

	class PointSet2D : public PointSet {
		public:
			PointSet2D(Point3f _center, float _radius = std::numeric_limits<float>::max(), int _k = std::numeric_limits<int>::max());

			bool insert(Vertex * v);
	};

	class PointKDTree2D : public KDTree<Vertex> {
		public:
			void collect(KDNode<Vertex> const * node, BoundingBox const & box, PointSet2D & pointSet) const;

		private:
			void divideNode(KDNode<Vertex> * node, BoundingBox & box, int depth);
			float nodeDistance(Point3f const & p, BoundingBox const & box) const;
	};


	class PointCloud2D : public PointCloud {
		public:
			void update();
			set<Vertex const *> collectKNearest(Point3f const & p, int k) const;
			set<Vertex const *> collectInRadius(Point3f const & p, float radius) const;
			set<Vertex const *> selection;

		private:
			PointKDTree2D kdTree;
	};
}
#endif // POINTCLOUD2D_H
