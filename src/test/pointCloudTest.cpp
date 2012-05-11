#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <iostream>
#include <boost/program_options.hpp>
#include <boost/foreach.hpp>

#include "cg2/PointCloud.hpp"
#include "tbd/log.h"


using namespace boost;
namespace po = program_options;

using namespace std;

LOG_INIT;

int main(int ac, char* av[])
{
  cout << "PointCloudTest -- written by Wilston Oreo." << endl;

  stringstream descStr;
  descStr << "Allowed options";

  string inputFile, outputFile;

  // Declare the supported options.
  po::options_description desc(descStr.str());

  desc.add_options()
    ("help,h", "Display help message.")
    ("input,i", po::value<string>(&inputFile), "Input OFF file")
    ("output,o", po::value<string>(&outputFile), "Output OFF file")
    ;

  // Parse the command line arguments for all supported options
  po::variables_map vm;
  po::store(po::parse_command_line(ac, av, desc), vm);
  po::notify(vm);

  if (vm.count("help"))
  {
    cout << desc << endl;
    return 1;
  }

  LOG->level(2);

  cg2::PointCloud pointCloud;
  pointCloud.read(inputFile);
  pointCloud.write(outputFile);

  cg2::Point3f p(1.0,1.0,0.0);
  pointCloud.collectKNearest(p,20);

  BOOST_FOREACH( const cg2::Vertex* vertex, pointCloud.selection )
    LOG_MSG << fmt("(%,%,%) dist = %") % vertex->v.x % vertex->v.y % vertex->v.z % (vertex->v - p).length();




  return EXIT_SUCCESS;
}

