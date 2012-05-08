#pragma once

#include <tbd/declare_property.h>

#include <limits>

namespace cg2 
{
  typedef unsigned char u8;
  typedef unsigned int u32;
  
  #define RND float(rand())/float(RAND_MAX)
  #define U8(a) (a) > 1.0 ? 255 : (a) < 0.0 ? 0 : (u8)((a)*255.0f)

  #define INF std::numeric_limits<double>::max()
}

