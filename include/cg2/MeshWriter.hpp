#pragma once

#include "cg2/Vertex.hpp"

namespace cg2 {
	class MeshWriter {
		public:
			virtual void write(std::string filename, vector<Vertex> * vertices = NULL, vector<Polygon> * polygons = NULL) = 0;
	};
}
