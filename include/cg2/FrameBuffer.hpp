#pragma once
#include "cg2/Image.hpp"

#include <vector>

namespace cg2
{
  using std::vector;

  class FrameBuffer
  {
    public:
      FrameBuffer(int _w = 0, int _h = 0): w_(_w), h_(_h), img(_w,_h), sampleBuffer(_w*_h,0) { };

      int w() const { return w_; }
      int h() const { return h_; }

      void h(int _h) { h_=_h; }
      void w(int _w) { w_=_w; }

      void set(float x, float y, Vec3f color)
      {
        int X = int(x), Y = int(y);
        //    cout << X << " " << Y << " " << w() << " " << h() << endl;
        if (X >= w() || Y >= h() || X < 0 || Y < 0) return;

        int pos = (h()-1-Y)*w() + X;
        sampleBuffer[pos]++;
        img.data()[pos] += color;
        //    cout << img._data[pos].x << " " << sampleBuffer[pos] << endl;
      }

      void drawToImage(Image& image)
      {
        image.resize(w_,h_);
        for (size_t i = 0; i < sampleBuffer.size(); i++) 
        {   
          if (sampleBuffer[i] == 0) continue;
          image.data()[i] = (1.0f / sampleBuffer[i]) * img.data()[i];
        }   
      }

      void resize(int _w, int _h) 
      {
        h(_h); w(_w);
        img.resize(_w,_h);
        sampleBuffer.resize(_w*_h,0);
        for (size_t i = 0; i < sampleBuffer.size(); i++)
          sampleBuffer[i] = 0;
      }

    private:
      int w_, h_; 
      Image img;
      vector<int> sampleBuffer;
  };
}

