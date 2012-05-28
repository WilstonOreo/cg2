#pragma once

#include "cg2/SceneObject.hpp"
#include "cg2/Vertex.hpp"

namespace cg2 {
	struct Mesh : public SceneObject {
			vector<Vertex> vertices;
			virtual void read(string const & filename) = 0;

			size_t size() const {
				return vertices.size();
			}

		protected:
			void calcBoundingBox();
	};
}
