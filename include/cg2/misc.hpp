#pragma once

#include <tbd/property.h>

#include <limits>

namespace cg2
{
  typedef unsigned char u8;
  typedef unsigned int u32;

  static double wendland(double d, double h)
  {
    /*x *= h;
      const double Q_PI = 3.14159265358979323846;
      return (1/sqrt(2*Q_PI))*exp(-0.5*d*d); // gauss, not wendland*/
    return std::pow(1-d/h, 4)*(4*d/h+1);
  }

#define RND float(rand())/float(RAND_MAX)
#define U8(a) (a) > 1.0 ? 255 : (a) < 0.0 ? 0 : (u8)((a)*255.0f)

#define INF std::numeric_limits<double>::max()
#define EPSILON 0.00001

#define FOREACH_AXIS for(int axis = 0; axis < 3; axis++)

#define DEFINE_CONTAINERS(TYPENAME) typedef std::vector<TYPENAME> TYPENAME##s; \
                                   typedef std::vector<TYPENAME*> TYPENAME##List;
}

