#pragma once

#include "cg2/MeshWriter.hpp"

namespace cg2 {
	class OFFWriter {
		public:
			void write(std::string filename, vector<Vertex> const  * vertices = NULL, vector<Polygon> const * polygons = NULL);
	};
}
