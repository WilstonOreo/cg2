#pragma once

#include <iostream>
#include <cmath>
#include "cg2/misc.hpp"

namespace cg2 {
	using std::istream;
	using std::ostream;
	using std::string;

	class Matrix4f {
		public:
			Matrix4f() {
				for (int i = 0; i < 4; i++)
					for (int j = 0; j < 4; j++) {
						set(i,j,(i == j) ? 1.0f : 0.0f);
					}
			};

			void  set(int _x, int _y, float v) {
				c[_x*4+_y] = v;
			}
			float get(int _x, int _y) {
				return c[_x*4+_y];
			}

			void  print();

		private:
			float c[4*4];
	};

	struct Coords3f {
		Coords3f(float _x = 0.0f, float _y = 0.0f, float _z = 0.0f) {
			set(_x,_y,_z);
		}

		void set(float _x, float _y, float _z) {
			x = _x;
			y = _y;
			z = _z;
		}
		float * p() {
			return cell;
		}
		const float * p() const {
			return cell;
		}
		float operator[](int i) const {
			return cell[i];
		}

		union {
			struct {
				float x,y,z;
			};
			struct {
				float cell[3];
			};
		};

	};

	struct Vec3f : public Coords3f {
		Vec3f(float _x = 0.0f, float _y = 0.0f, float _z = 0.0f) : Coords3f(_x,_y,_z) { }

		float    length()               {
			return (float)sqrtf(x * x + y * y + z * z);
		}
		void     normalize()            {
			float l = length();
			if (l>0.0f) {
				set(x/l,y/l,z/l);
			}
		}
		Vec3f    normalized()           {
			Vec3f v(x,y,z);
			v.normalize();
			return v;
		}

		Vec3f cross(Vec3f v)        {
			return Vec3f(y*v.z - z*v.y,
						 z*v.x - x*v.z,
						 x*v.y - y*v.x);
		}

		Vec3f operator- () const {
			return Vec3f(-x, -y, -z);
		}
		void operator *= (float f)     {
			x *= f;
			y *= f;
			z *= f;
		}

		friend Vec3f operator*(const Vec3f & a, float f) {
			return Vec3f(a.x*f, a.y*f, a.z*f);
		}

		friend Vec3f operator*(float f, const Vec3f & a) {
			return Vec3f(a.x*f, a.y*f, a.z*f);
		}
		friend float operator*(Vec3f & a, Vec3f & b) {
			return a.x*b.x + a.y*b.y + a.z*b.z;
		}
		friend Vec3f operator%(Vec3f & a, Vec3f & b) {
			return Vec3f(a.x*b.x,a.y*b.y,a.z*b.z);
		}

		friend Vec3f operator-(const Vec3f & a, const Vec3f & b) {
			return Vec3f(a.x-b.x, a.y-b.y, a.z-b.z);
		}
		friend Vec3f operator+(const Vec3f & a, const Vec3f & b) {
			return Vec3f(a.x+b.x, a.y+b.y, a.z+b.z);
		}

		friend Vec3f operator*(const Vec3f & a, Matrix4f & M) {
			Vec3f v;
			v.x = a.x * M.get(0,0) + a.y*M.get(1,0) + a.z*M.get(2,0) + M.get(3,0);
			v.y = a.x * M.get(0,1) + a.y*M.get(1,1) + a.z*M.get(2,1) + M.get(3,1);
			v.z = a.x * M.get(0,2) + a.y*M.get(1,2) + a.z*M.get(2,2) + M.get(3,2);
			return v;
		}

		void operator += (Vec3f v) {
			x += v.x;
			y += v.y;
			z += v.z;
		}
		void operator -= (Vec3f v) {
			x -= v.x;
			y -= v.y;
			z -= v.z;
		}
	};

	struct Point3f : public Coords3f {
		Point3f(float _x = 0.0f, float _y = 0.0f, float _z = 0.0f) : Coords3f(_x,_y,_z) { }

		void operator += (Point3f p) {
			x += p.x;
			y += p.y;
			z += p.z;
		}
		void operator -= (Point3f p) {
			x -= p.x;
			y -= p.y;
			z -= p.z;
		}

		friend Vec3f operator-(const Point3f & a, const Point3f & b) {
			return Vec3f(a.x-b.x, a.y-b.y, a.z-b.z);
		}
		friend Point3f operator+(const Point3f & a, const Point3f & b) {
			return Point3f(a.x+b.x, a.y+b.y, a.z+b.z);
		}
		friend Point3f operator+(const Point3f & a, const Vec3f & b) {
			return Point3f(a.x+b.x, a.y+b.y, a.z+b.z);
		}

		Vec3f vec3f() const {
			return Vec3f(x,y,z);
		}
	};

	struct TexCoords {
		TexCoords(float _u = 0.0, float _v = 0.0) : u(_u), v(_v) {}
		float u,v;
	};

	typedef enum { X,Y,Z } Axis;

	typedef Vec3f Color;
}

