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
   * @tparam Scalar Coordinate type
   */
  template<int DIMENSIONS, typename Scalar = DEFAULT_TYPE>  
  struct Coords
  {
    /// Coordinate value type
    typedef Scalar value_type;
    
    /// Base constructor, all values are initially set to zero
    Coords() { FOREACH_DIM a_[i] = 0; }
    Coords( Coords& _coords ) { FOREACH_DIM a_[i] = _coords[i]; }
    Coords( const Coords& _coords ) { FOREACH_DIM a_[i] = _coords[i]; }
    Coords( value_type _x, value_type _y) { this->operator()(_x,_y); }
    Coords( value_type _x, value_type _y, value_type _z) { this->operator()(_x,_y,_z); }
    Coords( value_type _x, value_type _y, value_type _z, value_type _w) { this->operator()(_x,_y,_z,_w); }
   
    inline void operator()(const Coords _coords) { FOREACH_DIM a_[i] = _coords[i]; } 
    inline void operator()(value_type _x, value_type _y) { x(_x); y(_y); }
    inline void operator()(value_type _x, value_type _y, value_type _z) { x(_x); y(_y); z(_z); }
    inline void operator()(value_type _x, value_type _y, value_type _z, value_type _w) { x(_x); y(_y); z(_z); w(_w); }
    
    /* @brief Return pointer */
    value_type* p() { return a_; }
    /* @brief Return const pointer */
    const value_type* p() const { return a_; }

    /// Methods to return coordinate values
    inline value_type x() const { BOOST_STATIC_ASSERT(DIMENSIONS >= 1); return a_[0]; }
    inline value_type y() const { BOOST_STATIC_ASSERT(DIMENSIONS >= 2); return a_[1]; }
    inline value_type z() const { BOOST_STATIC_ASSERT(DIMENSIONS >= 3); return a_[2]; }
    inline value_type w() const { BOOST_STATIC_ASSERT(DIMENSIONS >= 4); return a_[3]; }
    
    /// Methods to set coordinate values 
    inline void x(const value_type _x) { BOOST_STATIC_ASSERT(DIMENSIONS >= 1); a_[0] = _x; }
    inline void y(const value_type _y) { BOOST_STATIC_ASSERT(DIMENSIONS >= 2); a_[1] = _y; }
    inline void z(const value_type _z) { BOOST_STATIC_ASSERT(DIMENSIONS >= 3); a_[2] = _z; }
    inline void w(const value_type _w) { BOOST_STATIC_ASSERT(DIMENSIONS >= 4); a_[3] = _w; }

    /// Methods to access coordinate value in a certain dimension
    value_type& operator[] (int i) { return a_[i]; }
    const value_type& operator[] (int i) const { return a_[i]; }

    void operator += ( const Coords _c ) { FOREACH_DIM a_[i] += _c[i]; }
    void operator -= ( const Coords _c ) { FOREACH_DIM a_[i] -= _c[i]; } 

    /// size/dimension of the vector
    static const size_t size_ = DIMENSIONS;

    /// size/dimension of the vector
    static size_t size() { return 0; }


    protected:
    /// Array to store coordinate values
    Scalar a_[DIMENSIONS]; 
  };

  /** @brief Template class to represent a vector
   * @tparam DIMENSIONS Number of dimensions
   * @tparam Scalar Coordinate type
   */
  template<int DIMENSIONS, typename Scalar = DEFAULT_TYPE>  
  struct Vec : public Coords<DIMENSIONS,Scalar>
  {
    /// Coordinate value type
    typedef Scalar value_type;

    /// Type of base class
    typedef Coords<DIMENSIONS,Scalar> _Coords;
    typedef Vec<DIMENSIONS,Scalar> vector_type;

    Vec() : _Coords() {}
    Vec( Vec& v ) : _Coords( v ) {}
    Vec( const Vec& v ) : _Coords( v ) {}
    Vec( value_type _x, value_type _y ) : _Coords(_x,_y) { }
    Vec( value_type _x, value_type _y, value_type _z) : _Coords(_x,_y,_z) { }
    Vec( value_type _x, value_type _y, value_type _z, value_type _w ) : _Coords(_x,_y,_z,_w) { }

    Scalar  sqrLength()   const     { return dot(*this,*this); }
    Scalar	 length() 		const	  	{ return    sqrt( sqrLength() ); }
    vector_type& normalize() { Scalar l = length(); FOREACH_DIM this->a_[i] /= l; return *this; }
    vector_type 	     normalized() const		{ return *this * ( 1 / length() ); }

    /** @brief Calculates cross product by this and another Vec
     * @param _left first vector
     * @param _right second vector
     * @returns cross product vector
     * @details Number of dimension must be 3!
     **/
    friend Vec cross(const Vec& _left, const Vec& _right)
    {
      BOOST_STATIC_ASSERT(DIMENSIONS == 3);
      return Vec(_left.y() * _right.z() - _left.z() * _right.y(),
                 _left.z() * _right.x() - _left.x() * _right.z(),
                 _left.x() * _right.y() - _left.y() * _right.x() );
    }      
    
    /** @brief Calculates dot product by this and another Vec
     * @param _left first vector
     * @param _right second vector
     * @returns Coord value
     **/
    friend value_type dot( const Vec& _left, const Vec& _right)
    {
      value_type sum = 0;
      FOREACH_DIM 
        sum += _left[i] * _right[i];
      return sum;
    }

    vector_type vectorize(const Scalar& _s) { FOREACH_DIM _Coords::a_[i] = _s; return *this; }

    /// Vector operations
    Vec operator- () const { Vec v(*this);  FOREACH_DIM v[i] = -v[i];  return v; }
    void operator *= ( value_type f ) 	{  FOREACH_DIM this->a_[i] *= f; }

    friend Vec      operator*( const Vec& a, const value_type f ) { Vec v(a); FOREACH_DIM v[i] *= f; return v; }
    friend Vec      operator*( const value_type f, const Vec& a ) { return a*f; }
    friend Vec      operator*( const Vec& a, const Vec& b) { Vec v; FOREACH_DIM v[i] = a[i]*b[i]; return v; } 
    friend Vec      operator-( const Vec& a, const Vec& b) { Vec v; FOREACH_DIM v[i] = a[i]-b[i]; return v; }
    friend Vec      operator+( const Vec& a, const Vec& b) { Vec v; FOREACH_DIM v[i] = a[i]+b[i]; return v; }
/*
    friend Vec      operator*( const Vec& a, const Matrix<DIMENSIONS+1,value_type>& M ) 
    { 
      vector_type v;
      v.x = a.x * M.get(0,0) + a.y*M.get(1,0) + a.z*M.get(2,0) + M.get(3,0);
      v.y = a.x * M.get(0,1) + a.y*M.get(1,1) + a.z*M.get(2,1) + M.get(3,1);
      v.z = a.x * M.get(0,2) + a.y*M.get(1,2) + a.z*M.get(2,2) + M.get(3,2);
      return v;
    }
*/
  };

  /** @brief Template class to represent a point 
   * @tparam DIMENSIONS Number of dimensions
   * @tparam Scalar Coordinate type
   */
  template<int DIMENSIONS, typename Scalar = DEFAULT_TYPE>  
  struct Point : public Coords<DIMENSIONS,Scalar>
  {
    typedef Scalar value_type;
    typedef Vec<DIMENSIONS,Scalar> vector_type;
    typedef Coords<DIMENSIONS,Scalar> _Coords;

    Point() : _Coords() {}
    Point( Point& p ) : _Coords( p ) {}
    Point( const Point& p ) : _Coords( p ) {}
    Point( value_type _x, value_type _y ) : _Coords(_x,_y) { }
    Point( value_type _x, value_type _y, value_type _z ) : _Coords(_x,_y,_z) { }
    Point( value_type _x, value_type _y, value_type _z, value_type _w ) : _Coords(_x,_y,_z,_w) { }

    friend vector_type operator-( const Point& a, const Point& b) { vector_type v; FOREACH_DIM v[i] = a[i]-b[i]; return v; }
    friend Point operator+( const Point& a, const Point& b) { Point p; FOREACH_DIM p[i] = a[i] + b[i]; return p; }
    friend Point operator+( const Point& a, const vector_type& b) { Point p; FOREACH_DIM p[i] = a[i] + b[i]; return p; }

    /* @brief Transform point into a vector */
    vector_type vec() const { vector_type v; FOREACH_DIM v[i] = this->a_[i]; return v; }
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

  #define COORDS(C) C.x(),C.y(),C.z() 
}

#endif /* _VECTOR_HPP */
