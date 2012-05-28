#pragma once

#include "cg2/PolygonMesh.hpp"
#include "cg2/Triangle.hpp"
#include "cg2/KDTree.hpp"

namespace cg2 {
	class TriangleKDTree : public KDTree<Triangle> {
		public:
			TriangleKDTree();
			float recKDTreeTraverse(Ray & ray, Node * node, float tnear, float tfar, bool & found);

		private:
			void divideNode(Node * node, BoundingBox & box, int depth);
	};

	struct TriangleMesh : public PolygonMesh {
			void read(string filename);

			bool intersect(Ray & ray);

			TexCoords texCoords(const Ray & ray) {
				return TexCoords();
			}
			Vec3f normal(const Ray & ray) {
				return Vec3f();
			}

			TBD_DECLARE_PROPERTY_REF(Triangles,triangles);
		protected:
			void calcTriangles();

		private:

			TriangleKDTree kdTree;
	};
}
