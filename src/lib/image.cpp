#include <stdio.h>
#include <cstring>
#include <fstream>

#include "cg2/Image.hpp"

namespace cg2 {
	using std::ofstream;

	void Image::load_ppm(const std::string & filename) {
		FILE * fp;
		char head[70];

		fp = fopen(filename.c_str(), "rb");
		if (!fp) {
			std::cerr << "opening file " << filename << " failed" << std::endl;
			return;
		}

		// grab first two chars of the file and make sure that it has the
		// correct magic cookie for a raw PPM file.
		fgets(head, 70, fp);
		if (strncmp(head, "P6", 2) != 0) {
			std::cerr << "file " << filename << " is not a raw PPM file" << std::endl;
			return;
		}

		// grab the three elements in the header (width, height, maxval).
		int w, h, d;
		for (int i = 0; i < 3;) {
			fgets(head, 70, fp);
			if (head[0] == '#') {
				continue;
			}
			if (i == 0) {
				i += sscanf(head, "%d %d %d", &w, &h, &d);
			}
			else if (i == 1) {
				i += sscanf(head, "%d %d", &h, &d);
			}
			else if (i == 2) {
				i += sscanf(head, "%d", &d);
			}
		}

		// grab all the image data in one fell swoop.
		std::vector<u8> rawdata(width()*height()*3);
		fread(&rawdata[0], 1, rawdata.size(), fp);
		fclose(fp);

		data_.resize(w*h);
		for (unsigned i = 0; i < width()*height(); i++) {
			unsigned k = i * 3;
			data_[i] = Color(rawdata[k], rawdata[k+1], rawdata[k+2]) * (1.0f / 255.0f);
		}

		width(w);
		height(h);

		std::cout << "ppm read: image " << filename << " loaded, w=" << w << " h=" << h << std::endl;
	}


	void Image::save_ppm(const std::string & filename) {
		ofstream os(filename.c_str());
		os << "P6" << "\n";
		os << width() << " " << height() << "\n" << "255\n";

		for (unsigned i = 0; i < width()*height(); i++) {
			u8 r = U8(data_[i].x), g = U8(data_[i].y), b = U8(data_[i].z);
			os.write(reinterpret_cast<char *>(&r),sizeof(u8));
			os.write(reinterpret_cast<char *>(&g),sizeof(u8));
			os.write(reinterpret_cast<char *>(&b),sizeof(u8));
		}
	}
}
