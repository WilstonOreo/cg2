#pragma once

#include "cg2/Color.hpp"
#include "cg2/misc.hpp"

#include <vector>
#include <string>

namespace cg2
{
  using std::vector;

  struct Image
  {
    Image(unsigned _width = 0, unsigned _height = 0)
      : width_(_width), height_(_height)
    {
      resize(_width,_height);
    }


    Image(const std::string & filename)
    {
      load_ppm(filename);
    }

    void load_ppm(const std::string & filename);
    void save_ppm(const std::string & filename);

    void resize(unsigned _width, unsigned _height)
    {
      data_.resize(_width*_height);
      for (size_t i = 0; i < data_.size(); i++)
      {
        data_[i] = Color3f();
      }
    }

    inline Color3f & operator()(int x, int y)
    {
      return data_[x + y*width_];
    }

    inline const Color3f & operator()(int x, int y) const
    {
      return data_[x + y*width_];
    }

    inline const Color3f & get(int x, int y)
    {
      return data_[x + y*width_];
    }

    inline void set(int x, int y, Color3f color)
    {
      data_[x + y*width_] = color;
    }

    inline bool valid() const
    {
      return !data_.empty();
    }

    TBD_PROPERTY(unsigned,width);
    TBD_PROPERTY(unsigned,height);
    TBD_PROPERTY_REF(vector<Color3f>,data);
  };
}

