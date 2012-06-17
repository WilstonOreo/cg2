
#include "cg2/PointCloud.hpp"

namespace cg2 {


  struct Voxel
  {
    Voxel() : f_(0.0) {}

    float f_;
    Point3f center_,n_;
  };


  class ImpliciteSurface : public PointCloud
  { 
    public: 
    void read(string filename);

    void size(unsigned _x, unsigned _y, unsigned _z)
    {
      x_ = _x; y_ = _y; z_ = _z;
      voxels_.clear(); voxels_.resize(x_*y_*z_);

      Vec3f _voxelSize = voxelSize();

      for (unsigned x = 0; x < x_; x++)
        for (unsigned y = 0; y < y_; y++)
          for (unsigned z = 0; z < z_; z++)
          {
            Vec3f _pos(x,y,z);
            Vec3f v = boundingBox().min.vec3f() + _voxelSize % _pos + _voxelSize*0.5;
            voxel(x,y,z).center_.set(v.x,v.y,v.z);
          }

      calcBoundingBox();
    }

     Voxel& voxel(unsigned _posX, unsigned _posY, unsigned _posZ)  
    {
      return voxels_[ (( y_ * _posZ ) + _posY)  * x_ + _posX];
    }

     Voxel& voxel(const Point3f& _point) 
    {
      Vec3f invSize(1.0f/boundingBox().size().x*x_,
                    1.0f/boundingBox().size().y*y_,
                    1.0f/boundingBox().size().z*z_);
      Vec3f _org =  (_point - boundingBox().min); 
      Vec3f _p = _org % invSize;

      return voxel(unsigned(_p.x),unsigned(_p.y),unsigned(_p.z));
    }

    Vec3f voxelSize() const;

    void drawPoints(Color color = Color()) {}
    void drawValues(Color color = Color()) {}

  protected:
    void calcBoundingBox();
    void calcBorderConditions(float _epsilon, std::vector<float>& _f);

  private:
    void calcImplicit();

    unsigned x_, y_, z_;

    std::vector<Voxel> voxels_;
    std::vector<float> f_pN_, f_p2N_;
    
    float epsilon_;
  };
}
