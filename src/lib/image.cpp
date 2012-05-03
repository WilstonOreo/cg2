#include <stdio.h>
#include <cstring>
#include <fstream>
#include "image.hpp"
void Image::load_ppm(const std::string& filename)
{
    FILE* fp;
    char head[70];
    
    fp = fopen(filename.c_str(), "rb");
    if (!fp)
    {
        std::cerr << "opening file " << filename << " failed" << std::endl;
        return;
    }
    
    // grab first two chars of the file and make sure that it has the
    // correct magic cookie for a raw PPM file.
    fgets(head, 70, fp);
    if (strncmp(head, "P6", 2) != 0)
    {
        std::cerr << "file " << filename << " is not a raw PPM file" << std::endl;
        return;
    }
    
    // grab the three elements in the header (width, height, maxval).
    int w, h, d;
    for (int i = 0; i < 3;)
    {
        fgets(head, 70, fp);
        if (head[0] == '#')
            continue;
        if (i == 0)
            i += sscanf(head, "%d %d %d", &w, &h, &d);
        else if (i == 1)
            i += sscanf(head, "%d %d", &h, &d);
        else if (i == 2)
            i += sscanf(head, "%d", &d);
    }
    
    // grab all the image data in one fell swoop.
    std::vector<u8> rawdata(w*h*3);
    fread(&rawdata[0], 1, rawdata.size(), fp);
    fclose(fp);

    _data.resize(w*h);
    for (int i = 0; i < w*h; i++)
    {
        int k = i * 3;
        _data[i] = Color(rawdata[k], rawdata[k+1], rawdata[k+2]) * (1.0f / 255.0f);
    }
    
    _w = w;
    _h = h;

    std::cout << "ppm read: image " << filename << " loaded, w=" << w << " h=" << h << std::endl;
}


void Image::save_ppm(const std::string& filename)
{
	ofstream os(filename.c_str());
	os << "P6" << "\n";
	os << _w << " " << _h << "\n" << "255\n";

	for (int i = 0; i < _w*_h; i++)
	{
		u8 r = U8(_data[i].x), g = U8(_data[i].y), b = U8(_data[i].z);	
		os.write(reinterpret_cast<char*>(&r),sizeof(u8));
		os.write(reinterpret_cast<char*>(&g),sizeof(u8));
		os.write(reinterpret_cast<char*>(&b),sizeof(u8));
	}
}
