#include "cg2/Mesh.hpp"

#include <boost/foreach.hpp>

#include <tbd/log.h>

namespace cg2 {
	void Mesh::calcBoundingBox() {
		boundingBox().min.set(INF,INF,INF);
		boundingBox().max.set(-INF,-INF,-INF);

		BOOST_FOREACH(Vertex& vertex, *this) {
			Point3f v = vertex.v;
			for (int j = 0; j < 3; j++) {
				if (v.cell[j] > boundingBox().max.cell[j]) {
					boundingBox().max.cell[j] = v.cell[j];
				}
				if (v.cell[j] < boundingBox().min.cell[j]) {
					boundingBox().min.cell[j] = v.cell[j];
				}
			}
		}
	}
}
