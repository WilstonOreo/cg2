#pragma once

#include "cg2/SceneObject.hpp"
#include "cg2/Vertex.hpp"

namespace cg2 {
	struct Mesh : public SceneObject, public Vertices {
			virtual void read(string filename) = 0;

		protected:
			void calcBoundingBox();
	};
}
