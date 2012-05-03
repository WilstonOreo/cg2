#ifndef _VEC_HPP
#define _VEC_HPP

#include <iostream>
#include <sstream>
#include <cmath>

namespace cg2
{
  class Matrix4f
  {
    public:
      Matrix4f() 
      { 
        for (int i = 0; i < 4; i++) 
          for (int j = 0; j < 4; j++)
            set(i,j,(i == j) ? 1.0f : 0.0f);	
      };

      void  set(int _x, int _y, float v) { c[_x*4+_y] = v; } 
      float get(int _x, int _y) { return c[_x*4+_y]; }


      void  print() 
      {
        for (int i = 0; i < 4; i++)
        {
          for (int j = 0; j < 4; j++)
            cout << get(j,i) << "\t"; 
          cout << endl;
        }
      }

    private:
      float c[4*4];
  };

  struct point3f
  {
    point3f( float _x = 0.0f, float _y = 0.0f, float _z = 0.0f ) { set(_x,_y,_z); }

    void set(float _x, float _y, float _z) { x = _x; y = _y; z = _z; }
    float* p() { return cell; }
    const float* p() const { return cell; }
    float operator[] (int i) { return cell[i]; }

    void operator += ( point3f p ) { x += p.x; y += p.y; z += p.z; }
    void operator -= ( point3f p ) { x -= p.x; y -= p.y; z -= p.z; }

    union 	
    {
      struct { float x,y,z; };
      struct { float cell[3]; };
    };

    friend point3f operator-( const point3f& a, const point3f& b) { return point3f(a.x-b.x, a.y-b.y, a.z-b.z); }
    friend point3f operator+( const point3f& a, const point3f& b) { return point3f(a.x+b.x, a.y+b.y, a.z+b.z); }
  };

  struct vec3f : public point3f
  {
    vec3f( float _x = 0.0f, float _y = 0.0f, float _z = 0.0f ) : point3f(_x,_y,_z) { }

    string 	 toString() { stringstream ss; ss << "(" << x << "," << y << "," << z << ")"; return ss.str(); }
    void 	 fromString(string& s);
    float 	 length() 				{ return (float)sqrtf( x * x + y * y + z * z ); }
    void 	 normalize() 			{ float l = length(); if (l>0.0f) set(x/l,y/l,z/l); }
    vec3f 	 normalized() 			{ vec3f v(x,y,z); v.normalize(); return v; }

    vec3f cross(vec3f v) 		{ return vec3f(y*v.z - z*v.y, 
        z*v.x - x*v.z, 
        x*v.y - y*v.x ); }

    vec3f operator- () const { return vec3f( -x, -y, -z ); }
    void operator *= ( float f ) 	{ x *= f; y *= f; z *= f; }

    friend vec3f operator*( const vec3f& a, float f ) { return vec3f(a.x*f, a.y*f, a.z*f); }

    friend vec3f operator*( float f, const vec3f& a ) { return vec3f(a.x*f, a.y*f, a.z*f); }
    friend float operator*( vec3f& a, vec3f& b) { return a.x*b.x + a.y*b.y + a.z*b.z; } 
    friend vec3f operator%( vec3f& a, vec3f& b) { return vec3f(a.x*b.x,a.y*b.y,a.z*b.z); } 

    friend vec3f operator-( const vec3f& a, const vec3f& b) { return vec3f(a.x-b.x, a.y-b.y, a.z-b.z); }
    friend vec3f operator+( const vec3f& a, const vec3f& b) { return vec3f(a.x+b.x, a.y+b.y, a.z+b.z); }

    friend vec3f operator*( const vec3f& a, Matrix4f& M ) 
    { 
      vec3f v;
      v.x = a.x * M.get(0,0) + a.y*M.get(1,0) + a.z*M.get(2,0) + M.get(3,0);
      v.y = a.x * M.get(0,1) + a.y*M.get(1,1) + a.z*M.get(2,1) + M.get(3,1);
      v.z = a.x * M.get(0,2) + a.y*M.get(1,2) + a.z*M.get(2,2) + M.get(3,2);
      return v;
    }

  };
}



#endif
