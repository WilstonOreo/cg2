#include "cg2/OFFReader.hpp"

#include <fstream>
#include <boost/algorithm/string/trim.hpp>
#include <boost/algorithm/string/split.hpp>

#include <tbd/log.h>

using namespace std;

namespace cg2 {
	void OFFReader::read(std::string filename, vector<Vertex> * vertices, vector<Polygon> * polygons) {
		ifstream is(filename.c_str(), ios::in);
		LOG_MSG << fmt("Reading mesh from % ...") % filename;

		enum Mode { HEADER, SIZE, VERTICES, POLYGONS, END };
		int nRow = 0;
		Mode mode = HEADER;

		if (vertices) {
			vertices->clear();
		}
		if (polygons) {
			polygons->clear();
		}
		int V = 0,F = 0,E = 0;

		while (is.good() && mode != END) {
			string row;
			std::getline(is, row);
			boost::trim(row);
			if (row.empty()) {
				continue;
			}

			vector<string> tokens;
			boost::split(tokens, row, boost::is_any_of("\t "), boost::token_compress_on);

			switch (mode) {
				case HEADER:
					if (row=="OFF" || row=="NOFF") {
						mode = SIZE;
						nRow++;
					}
					break;

				case SIZE:
					V = atoi(tokens[0].c_str());
					if (vertices) {
						vertices->reserve(V);
					}
					F = atoi(tokens[1].c_str());
					if (polygons) {
						polygons->reserve(F);
					}
					E = atoi(tokens[2].c_str());
					LOG_MSG << fmt("Found % vertices, % polygons and % edges.") % V % F % E;
					nRow++;
					mode = VERTICES;
					break;

				case VERTICES:
					if (vertices) {
						Vertex v;
						v.v.set(atof(tokens[0].c_str()),atof(tokens[1].c_str()),atof(tokens[2].c_str()));
						vertices->push_back(v);
						nRow++;
						if (nRow >= V+2) {
							mode = POLYGONS;
						}
						break;
					}
				case POLYGONS:
					if (polygons) {
						int n = atoi(tokens[0].c_str());
						if (n > 0) {
							vector<Vertex*> polygonVertices;
							polygonVertices.resize(n);
							for (int i = 0; i < n; i++) {
								polygonVertices[i] = &vertices->at(atoi(tokens[i+1].c_str()));
							}

							polygons->push_back(Polygon(polygonVertices));
							nRow++;
						}
						if (nRow >= V+F+2) {
							mode = END;
						}
						break;
					}
					else {
						mode = END;
					}
				case END:
					LOG_MSG_(2) << "End of file reached.";
					break;
			}
		}

		LOG_MSG_(1) << fmt("Loaded % vertices, % polygons and % edges") % (vertices ? vertices->size() : 0) %
					(polygons ? polygons->size() : 0) % 0;
	}
}


