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

	template<class T> class mathvector : public vector<T> {
		public:
			mathvector() {}
			mathvector(mathvector const & other) : vector<T>(other) {}
			explicit mathvector(size_t __n, const T& __value = T()) : vector<T>(__n,__value) {}

			mathvector & operator+=(mathvector const & rhs) {
				for (typename vector<T>::size_type i = 0; i < std::min(this->size(), rhs.size()); ++i) {
					(*this)[i] += rhs[i];
				}
				return *this;
			}
			mathvector & operator-=(mathvector const & rhs) {
				for (typename vector<T>::size_type i = 0; i < std::min(this->size(), rhs.size()); ++i) {
					(*this)[i] -= rhs[i];
				}
				return *this;
			}

			mathvector & operator*=(double rhs) {
				for (typename vector<T>::size_type i = 0; i < this->size(); ++i) {
					(*this)[i] *= rhs;
				}
				return *this;
			}

			friend mathvector<T> operator+(mathvector<T> const &lhs, mathvector<T> const &rhs) { mathvector<T> temp(lhs); temp += rhs; return temp; }
			friend mathvector<T> operator-(mathvector<T> const &lhs, mathvector<T> const &rhs) { mathvector<T> temp(lhs); temp -= rhs; return temp; }
			friend mathvector<T> operator*(mathvector<T> const &lhs, double rhs) { mathvector<T> temp(lhs); temp *= rhs; return temp; }
	};

	class PointCloud2D : public PointCloud {
		public:
			PointCloud2D() : width_(1), height_(1) {}

			void generateGrid(PointCloud const & in);
			void generateCasteljauGrid(PointCloud2D const & in);
			void update();
			set<Vertex const *> collectKNearest(Point3f const & p, int k) const;
			set<Vertex const *> collectInRadius(Point3f const & p, float radius) const;
			set<Vertex const *> selection;

			void drawSurface(Color const & color = Color());

			Vertex & getVertex(size_t xp, size_t yp) {
				if (xp < 0) xp = 0;
				if (yp < 0) yp = 0;
				if (xp >= width_) xp = width_-1;
				if (yp >= height_) yp = height_-1;

				return vertices[xp*height_+yp];
			}
			Vertex const & getVertex(size_t xp, size_t yp) const {
				if (xp < 0) xp = 0;
				if (yp < 0) yp = 0;
				if (xp >= width_) xp = width_-1;
				if (yp >= height_) yp = height_-1;

				return vertices[xp*height_+yp];
			}

			TBD_DECLARE_PROPERTY(size_t,width);
			TBD_DECLARE_PROPERTY(size_t,height);
		private:
			PointKDTree2D kdTree;
	};


}
#endif // POINTCLOUD2D_H
