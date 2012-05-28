#pragma once

#include "cg2/Vertex.hpp"

namespace cg2 {
	class MeshWriter {
		public:
			virtual void write(std::string filename, Vertices * vertices = NULL, Polygons * polygons = NULL) = 0;
	};
}
