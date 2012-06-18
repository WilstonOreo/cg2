#pragma once

#include "cg2/Ray.hpp"

namespace cg2
{
  struct Camera
  {
    Camera() {};
    Camera(Matrix4f & _m)
    {
      m=_m;
    }

    void setMatrix(double _m[16])
    {
      for (int x = 0; x < 4; x++)
        for (int y = 0; y < 4; y++)
        {
          m.set(x,y,_m[y*4+x]);
        }
    }

    Ray generateRay(float x, float y);
    Rays generatePrimaryRays(int scrWidth, int scrHeight);

    Matrix4f m;
    //float znear,zfar;
  };

}
