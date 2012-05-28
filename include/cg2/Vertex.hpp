#pragma once

#include "cg2/Vector.hpp"

#include <vector>
#include <set>

namespace cg2 {
	using std::vector;
	using std::set;

	struct Vertex {
		Vertex(Point3f _v = Point3f(), Vec3f _n = Vec3f()): v(_v), n(_n) {}
		void set(Point3f _v, Vec3f _n = Vec3f()) {
			v = _v;
			n = _n;
		}

		Point3f v;
		Vec3f n;
		bool hasNormal() {
			return (n.x != 0.0f) || (n.y != 0.0f) || (n.z != 0.0f);
		}
	};

	struct Polygon {
		vector<Vertex *> vertices;
		Polygon(vector<Vertex *> const & vertices) {
			this->vertices = vertices;
		}

		Polygon(int n = 3) {
			if (n < 3) {
				n = 3;
			}
			vertices.resize(n);
		}

		size_t size() const {
			return vertices.size();
		}

		Vec3f normal();
	};
}
