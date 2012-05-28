#pragma once

#include "cg2/MeshReader.hpp"

namespace cg2 {
	class OFFReader {
		public:
			void read(std::string filename, Vertices * vertices = NULL, Polygons * polygons = NULL);
	};
}
