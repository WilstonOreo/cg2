#include "cg2/OFFReader.hpp"

#include <fstream>
#include <boost/algorithm/string/trim.hpp>
#include <boost/algorithm/string/split.hpp>

#include <tbd/log.h>

using namespace std; 

namespace cg2 
{
  void OFFReader::read(std::string filename, vector<Vertex>* vertices, vector<Triangle>* triangles)
  {
    ifstream is(filename.c_str(), ios::in);
    LOG_MSG << fmt("Reading mesh from % ...") % filename;

    enum Mode { HEADER, SIZE, VERTICES, TRIANGLES, END };
    int nRow = 0;
    Mode mode = HEADER;
    
    if (vertices) vertices->clear();
    if (triangles) triangles->clear();
    int V = 0,F = 0,E = 0;

    while (is.good() && mode != END)
    {
      string row;
      std::getline(is, row);
      boost::trim(row);
      if (row.empty())
        continue;

      vector<string> tokens;
      boost::split(tokens, row, boost::is_any_of("\t "), boost::token_compress_on );

      switch(mode)
      {
        case HEADER:
          if (row=="OFF") { mode = SIZE; nRow++; }
          break;

        case SIZE: 
          V = atoi(tokens[0].c_str()); if (vertices) vertices->reserve(V);
          F = atoi(tokens[1].c_str()); if (triangles) triangles->reserve(F);
          E = atoi(tokens[2].c_str());
          LOG_MSG << fmt("Found % vertices, % triangles and % edges.") % V % F % E;
          nRow++; mode = VERTICES;
          break;

        case VERTICES: if (vertices) 
                       {
                         Vertex v; v.v(atof(tokens[0].c_str()),atof(tokens[1].c_str()),atof(tokens[2].c_str()));
                         vertices->push_back(v);
                         nRow++;
                         if (nRow >= V+2) mode = TRIANGLES;
                         break;
                       }
        case TRIANGLES: if (triangles) 
                       {
                         int n = atoi(tokens[0].c_str());
                         if (n == 3)
                         {
                           Triangle tri(vertices->at(atoi(tokens[1].c_str())).v,
                                        vertices->at(atoi(tokens[2].c_str())).v,
                                        vertices->at(atoi(tokens[3].c_str())).v);
                           triangles->push_back(tri);
                           nRow++;
                         }
                         if (nRow >= V+F+2) mode = END;
                         break;
                       } else mode = END;
        case END:
                       LOG_MSG_(2) << "End of file reached.";
                       break;
      }
    }

    LOG_MSG_(1) << fmt("Loaded % vertices, % triangles and % edges") % (vertices ? vertices->size() : 0) %
                                                                      (triangles ? triangles->size() : 0) % 0;
  }
  
  void OFFReader::read(std::string filename, vector<Vertex>* vertices, vector<VertexTriangle>* triangles)
  {
    ifstream is(filename.c_str(), ios::in);
    LOG_MSG << fmt("Reading mesh from % ...") % filename;

    enum Mode { HEADER, SIZE, VERTICES, TRIANGLES, END };
    int nRow = 0;
    Mode mode = HEADER;
    
    if (vertices) vertices->clear();
    if (triangles) triangles->clear();
    int V = 0,F = 0,E = 0;

    while (is.good() && mode != END)
    {
      string row;
      std::getline(is, row);
      boost::trim(row);
      if (row.empty())
        continue;

      vector<string> tokens;
      boost::split(tokens, row, boost::is_any_of("\t "), boost::token_compress_on );

      switch(mode)
      {
        case HEADER:
          if (row=="OFF") { mode = SIZE; nRow++; }
          break;

        case SIZE: 
          V = atoi(tokens[0].c_str()); if (vertices) vertices->reserve(V);
          F = atoi(tokens[1].c_str()); if (triangles) triangles->reserve(F);
          E = atoi(tokens[2].c_str());
          LOG_MSG << fmt("Found % vertices, % triangles and % edges.") % V % F % E;
          nRow++; mode = VERTICES;
          break;

        case VERTICES: if (vertices) 
                       {
                         Vertex v; v.v(atof(tokens[0].c_str()),atof(tokens[1].c_str()),atof(tokens[2].c_str()));
                         vertices->push_back(v);
                         nRow++;
                         if (nRow >= V+2) mode = TRIANGLES;
                         break;
                       }
        case TRIANGLES: if (triangles) 
                       {
                         int n = atoi(tokens[0].c_str());
                         if (n == 3)
                         {
                           VertexTriangle tri(&vertices->at(atoi(tokens[1].c_str())),
                                              &vertices->at(atoi(tokens[2].c_str())),
                                              &vertices->at(atoi(tokens[3].c_str())));
                           triangles->push_back(tri);
                           nRow++;
                         }
                         if (nRow >= V+F+2) mode = END;
                         break;
                       } else mode = END;
        case END:
                       LOG_MSG_(2) << "End of file reached.";
                       break;
      }
    }

    LOG_MSG_(1) << fmt("Loaded % vertices, % triangles and % edges") % (vertices ? vertices->size() : 0) %
                                                                      (triangles ? triangles->size() : 0) % 0;
  }

}


