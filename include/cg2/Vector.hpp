#ifndef _VECTOR_HPP
#define _VECTOR_HPP

#include <iostream>
#include <cmath>
#include "cg2/misc.hpp"
#include <boost/static_assert.hpp>

namespace cg2
{
  using std::istream;
  using std::ostream;
  using std::string;

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

      void  print();

    private:
      float c[4*4];
  };

  /// DEFAULT_TYPE of coordinates is float
  typedef float DEFAULT_TYPE;

/// Compiler macro to traverse through each dimension
#define FOREACH_DIM for (int i = 0; i < DIMENSIONS; i++) 

  /** @brief Base class of Point and Vec which basically hold a number of coordinates
   * @tparam DIMENSIONS Number of dimensions
   * @tparam COORD_TYPE Coordinate type
   */
  template<int DIMENSIONS, typename COORD_TYPE = DEFAULT_TYPE>  
  struct Coords
  {
    /// Coordinate value type
    typedef COORD_TYPE CoordType;
    
    /// Base constructor, all values are initially set to zero
    Coords() { FOREACH_DIM a_[i] = 0; }
    Coords( Coords& _coords ) { FOREACH_DIM a_[i] = _coords[i]; }
    Coords( const Coords& _coords ) { FOREACH_DIM a_[i] = _coords[i]; }
    Coords( CoordType _x, CoordType _y) { this->operator()(_x,_y); }
    Coords( CoordType _x, CoordType _y, CoordType _z) { this->operator()(_x,_y,_z); }
    Coords( CoordType _x, CoordType _y, CoordType _z, CoordType _w) { this->operator()(_x,_y,_z,_w); }
   
    inline void operator()(const Coords _coords) { FOREACH_DIM a_[i] = _coords[i]; } 
    inline void operator()(CoordType _x, CoordType _y) { x(_x); y(_y); }
    inline void operator()(CoordType _x, CoordType _y, CoordType _z) { x(_x); y(_y); z(_z); }
    inline void operator()(CoordType _x, CoordType _y, CoordType _z, CoordType _w) { x(_x); y(_y); z(_z); w(_w); }
    
    /* @brief Return pointer */
    CoordType* p() { return a_; }
    /* @brief Return const pointer */
    const CoordType* p() const { return a_; }

    /// Methods to return coordinate values
    inline CoordType x() const { BOOST_STATIC_ASSERT(DIMENSIONS >= 1); return a_[0]; }
    inline CoordType y() const { BOOST_STATIC_ASSERT(DIMENSIONS >= 2); return a_[1]; }
    inline CoordType z() const { BOOST_STATIC_ASSERT(DIMENSIONS >= 3); return a_[2]; }
    inline CoordType w() const { BOOST_STATIC_ASSERT(DIMENSIONS >= 4); return a_[3]; }
    
    /// Methods to set coordinate values 
    inline void x(const CoordType _x) { BOOST_STATIC_ASSERT(DIMENSIONS >= 1); a_[0] = _x; }
    inline void y(const CoordType _y) { BOOST_STATIC_ASSERT(DIMENSIONS >= 2); a_[1] = _y; }
    inline void z(const CoordType _z) { BOOST_STATIC_ASSERT(DIMENSIONS >= 3); a_[2] = _z; }
    inline void w(const CoordType _w) { BOOST_STATIC_ASSERT(DIMENSIONS >= 4); a_[3] = _w; }

    /// Methods to access coordinate value in a certain dimension
    CoordType& operator[] (int i) { return a_[i]; }
    const CoordType& operator[] (int i) const { return a_[i]; }

    void operator += ( const Coords _c ) { FOREACH_DIM a_[i] += _c[i]; }
    void operator -= ( const Coords _c ) { FOREACH_DIM a_[i] -= _c[i]; } 

    protected:
    /// Array to store coordinate values
    CoordType a_[DIMENSIONS]; 
  };

  /** @brief Template class to represent a vector
   * @tparam DIMENSIONS Number of dimensions
   * @tparam COORD_TYPE Coordinate type
   */
  template<int DIMENSIONS, typename COORD_TYPE = DEFAULT_TYPE>  
  struct Vec : public Coords<DIMENSIONS,COORD_TYPE>
  {
    /// Coordinate value type
    typedef COORD_TYPE CoordType;

    /// Type of base class
    typedef Coords<DIMENSIONS,COORD_TYPE> _Coords;

    Vec() : _Coords() {}
    Vec( Vec& v ) : _Coords( v ) {}
    Vec( const Vec& v ) : _Coords( v ) {}
    Vec( CoordType _x, CoordType _y ) : _Coords(_x,_y) { }
    Vec( CoordType _x, CoordType _y, CoordType _z) : _Coords(_x,_y,_z) { }
    Vec( CoordType _x, CoordType _y, CoordType _z, CoordType _w ) : _Coords(_x,_y,_z,_w) { }

    CoordType  sqrLength()        { return this->dot(*this); }
    CoordType	 length() 			  	{ return    sqrt( sqrLength() ); }
    void 	     normalize() 	  		{ CoordType l = length(); if (l>0.0f) FOREACH_DIM this->a_[i] /= l; }
    Vec 	     normalized() const { Vec v(*this); v.normalize(); return v; }

    /* @brief Calculates cross product by this and another Vec
     * @param v   Second vector
     * @returns Vector
     * @details Number of dimension must be 3!
     **/
    const Vec cross(const Vec& v) const
    { 
      BOOST_STATIC_ASSERT(DIMENSIONS == 3);
      return Vec(this->y()*v.z() - this->z()*v.y(), 
                 this->z()*v.x() - this->x()*v.z(), 
                 this->x()*v.y() - this->y()*v.x() ); 
    }

    /* @brief Calculates dot product by this and another Vec
     * @param v   Second vector
     * @returns CoordType value
     **/
    const CoordType dot( const Vec& v) const 
    { 
      CoordType sum = 0; 
      FOREACH_DIM sum += v[i]*this->a_[i]; 
      return sum; 
    } 

    /// Vector operations
    Vec operator- () const { Vec v(*this);  FOREACH_DIM v[i] = -v[i];  return v; }
    void operator *= ( CoordType f ) 	{  FOREACH_DIM this->a_[i] *= f; }

    friend Vec      operator*( const Vec& a, const CoordType f ) { Vec v(a); FOREACH_DIM v[i] *= f; return v; }
    friend Vec      operator*( const CoordType f, const Vec& a ) { return a*f; }
    friend Vec      operator*( const Vec& a, const Vec& b) { Vec v; FOREACH_DIM v[i] = a[i]*b[i]; return v; } 
    friend Vec      operator-( const Vec& a, const Vec& b) { Vec v; FOREACH_DIM v[i] = a[i]-b[i]; return v; }
    friend Vec      operator+( const Vec& a, const Vec& b) { Vec v; FOREACH_DIM v[i] = a[i]+b[i]; return v; }
/*
    friend Vec      operator*( const Vec& a, const Matrix<DIMENSIONS+1,CoordType>& M ) 
    { 
      _Vec v;
      v.x = a.x * M.get(0,0) + a.y*M.get(1,0) + a.z*M.get(2,0) + M.get(3,0);
      v.y = a.x * M.get(0,1) + a.y*M.get(1,1) + a.z*M.get(2,1) + M.get(3,1);
      v.z = a.x * M.get(0,2) + a.y*M.get(1,2) + a.z*M.get(2,2) + M.get(3,2);
      return v;
    }
*/
  };

  /** @brief Template class to represent a point 
   * @tparam DIMENSIONS Number of dimensions
   * @tparam COORD_TYPE Coordinate type
   */
  template<int DIMENSIONS, typename COORD_TYPE = DEFAULT_TYPE>  
  struct Point : public Coords<DIMENSIONS,COORD_TYPE>
  {
    typedef COORD_TYPE CoordType;
    typedef Vec<DIMENSIONS,COORD_TYPE> _Vec;
    typedef Coords<DIMENSIONS,COORD_TYPE> _Coords;

    Point() : _Coords() {}
    Point( Point& p ) : _Coords( p ) {}
    Point( const Point& p ) : _Coords( p ) {}
    Point( CoordType _x, CoordType _y ) : _Coords(_x,_y) { }
    Point( CoordType _x, CoordType _y, CoordType _z ) : _Coords(_x,_y,_z) { }
    Point( CoordType _x, CoordType _y, CoordType _z, CoordType _w ) : _Coords(_x,_y,_z,_w) { }

    friend _Vec operator-( const Point& a, const Point& b) { _Vec v; FOREACH_DIM v[i] = a[i]-b[i]; return v; }
    friend Point operator+( const Point& a, const Point& b) { Point p; FOREACH_DIM p[i] = a[i] + b[i]; return p; }
    friend Point operator+( const Point& a, const _Vec& b) { Point p; FOREACH_DIM p[i] = a[i] + b[i]; return p; }

    /* @brief Transform point into a vector */
    _Vec vec() const { _Vec v; FOREACH_DIM v[i] = this->a_[i]; return v; }
  };

#undef FOREACH_DIM

  typedef enum { X,Y,Z,W } Axis;

  typedef Vec<2,int> Vec2i;
  typedef Point<2,int> Point2i;
  typedef Vec<2,double> Vec2d;
  typedef Point<2,double> Point2d;

  typedef Vec<2,float> Vec2f;
  typedef Point<2,float> Point2f;
  typedef Vec<3,float> Vec3f;
  typedef Point<3,float> Point3f;
  //typedef Matrix<float> Matrix4f;

  #define COORDS(C) C.x(),C.y(),C.y() 
}

#endif /* _VECTOR_HPP */
