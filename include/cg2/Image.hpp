#ifndef _IMAGE_HPP
#define _IMAGE_HPP

#include "cg2/Vector.hpp"
#include "cg2/misc.hpp"

#include <vector>

namespace cg2
{
  using std::vector;
  typedef Vec3f Color;

  struct Image
  {
    Image(unsigned _width = 0, unsigned _height = 0)
      : width_(_width), height_(_height)
    {
      resize(_width,_height);
    }


    Image(const std::string& filename)
    {
      load_ppm(filename);
    }

    void load_ppm(const std::string& filename);
    void save_ppm(const std::string& filename);

    void resize(unsigned _width, unsigned _height)
    {
      data_.resize(_width*_height);
      for (size_t i = 0; i < data_.size(); i++)
        data_[i] = Color();
    }

    inline Color& operator() (int x, int y)
    {
      return data_[x + y*width_];
    }

    inline const Color& operator() (int x, int y) const
    {
      return data_[x + y*width_];
    }

    inline const Color& get(int x, int y) 
    {
      return data_[x + y*width_];
    }

    inline void set(int x, int y, Color color)
    {
      data_[x + y*width_] = color; 
    }

    inline bool valid() const
    {
      return !data_.empty();
    }

    TBD_DECLARE_PROPERTY(unsigned,width);
    TBD_DECLARE_PROPERTY(unsigned,height);
    TBD_DECLARE_PROPERTY_REF(vector<Color>,data);
  };
}
#endif
