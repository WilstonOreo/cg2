#pragma once

#include <boost/static_assert.hpp>

namespace cg2
{
  typedef unsigned char   u8;
  typedef unsigned short  u16;
  typedef unsigned int    u32;

/// Compiler macro to traverse through each channel
#define FOREACH_CHANNEL for (int i = 0; i < CHANNELS; i++) 

  template<int CHANNELS, class VALUE> 
  struct Color
  {
    static const int channels_ = CHANNELS;
    typedef VALUE Value;
    static const Value maxValue() { return 1; }
    Color()
    {
      BOOST_STATIC_ASSERT(channels_ >= 3 && channels_ <= 4);
      FOREACH_CHANNEL v_[i] = 0;
    }
    Color(Value _r, Value _g, Value _b )
    {
      BOOST_STATIC_ASSERT(channels_ >= 3 && channels_ <= 4);
      this->operator()(_r,_g,_b); 
      if( CHANNELS > 3 )
        v_[3] = maxValue();
    }
    Color(Value _r, Value _g, Value _b, Value _a )
    {
      BOOST_STATIC_ASSERT(channels_ == 4);
      this->operator()(_r,_g,_b,_a); 
    }
    const Value& r() const
    {
      return v_[0];
    }
    void r( const Value& _value ) 
    { 
      v_[0] = _value;
    }
    const Value& g() const
    {
      return v_[1];
    }
    void g( const Value& _value ) 
    { 
      v_[1] = _value;
    }
    const Value& b() const
    {
      return v_[2];
    }
    void b( const Value& _value ) 
    { 
      v_[2] = _value;
    }
    const Value& a() const
    {
      BOOST_STATIC_ASSERT(channels_ >= 4);
      return v_[3];
    }
    void a( const Value& _value ) 
    { 
      BOOST_STATIC_ASSERT(channels_ >= 4);
      v_[3] = _value;
    }

    const Value* values() const { return v_; }
    Value* values() { return v_; }
    operator const Value*() const { return v_; }
    operator Value*() { return v_; }

    void operator += ( const Color _c ) { FOREACH_CHANNEL v_[i] += _c[i]; }
    void operator -= ( const Color _c ) { FOREACH_CHANNEL v_[i] -= _c[i]; } 
    void operator *= ( Value f ) 	{  FOREACH_CHANNEL v_[i] *= f; }

    friend Color      operator*( const Color& _a, const Value _f ) { Color v(_a); FOREACH_CHANNEL v[i] *= _f; return v; }
    friend Color      operator*( const Value _f, const Color& _a ) { return _a*_f; }
    friend Color      operator*( const Color& _a, const Color& _b) { Color v; FOREACH_CHANNEL v[i] = _a[i]*_b[i]; return v; } 
    friend Color      operator-( const Color& _a, const Color& _b) { Color v; FOREACH_CHANNEL v[i] = _a[i]-_b[i]; return v; }
    friend Color      operator+( const Color& _a, const Color& _b) { Color v; FOREACH_CHANNEL v[i] = _a[i]+_b[i]; return v; }
    
    inline void operator()(const Color _color) { FOREACH_CHANNEL v_[i] = _color[i]; } 
    inline void operator()(Value _r, Value _g, Value _b) { r(_r); g(_g); b(_b); }
    inline void operator()(Value _r, Value _g, Value _b, Value _a) { r(_r); g(_g); b(_b); a(_a); }
  
    protected:

    Value v_[channels_];
  };

#undef FOREACH_CHANNEL

  typedef Color<3,float> Color3f;
  typedef Color<4,float> Color4f;
  typedef Color<3,u8> Color3ub;
  typedef Color<4,u8> Color4ub;
  typedef Color<3,u16> Color3us;
  typedef Color<4,u16> Color4us;
  typedef Color<3,u32> Color3u;
  typedef Color<4,u32> Color4u;
}

