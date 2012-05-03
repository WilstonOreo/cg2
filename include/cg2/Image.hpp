#ifndef _IMAGE_HPP
#define _IMAGE_HPP

#include <vector>
#include <string>
#include "cg2/Vector.hpp"
#include "cg2/misc.hpp"


typedef vec3f Color;

struct Image
{
    Image() : _w(0), _h(0)
    {
    }

    Image(int width, int height)
     : _w(width)
     , _h(height)
     , _data(width*height)
    {
    }

	TBD_DECLARE_PROPERTY(unsigned,width);
	TBD_DECLARE_PROPERTY(unsigned,height);
	TBD_DECLARE_PROPERTY_REF(vector<Color>,data);


    Image(const std::string& filename)
    {
        load_ppm(filename);
    }

    void load_ppm(const std::string& filename);
	void save_ppm(const std::string& filename);

	void resize(int width, int height)
	{
		_data.resize(width*height);
		for (size_t i = 0; i < _data.size(); i++)
			_data[i] = Color();
	}

    inline Color& operator() (int x, int y)
    {
        return _data[x + y*_w];
    }

    inline const Color& operator() (int x, int y) const
    {
        return _data[x + y*_w];
    }

	inline const Color& get(int x, int y) 
	{
        return _data[x + y*_w];
	}

	inline void set(int x, int y, Color color)
	{
		_data[x + y*_w] = color; 
	}

    inline bool valid() const
    {
        return !_data.empty();
    }
};

#endif
