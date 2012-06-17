
#include "cg2/PointCloud.hpp"

namespace cg2 {


  struct Voxel
  {
    Voxel() : f_(0.0), empty_(true) {}

    float f_;
    Point3f center_;
    Vec3f n_;
    bool empty_;
  };


  class ImpliciteSurface : public PointCloud 
  { 
    public: 
    void read(string filename);

    void size(unsigned _x, unsigned _y, unsigned _z);

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

    void draw(Color color = Color()) const;
    void drawPoints(Color color, Point3f _lightPos) const;
    void drawValues(Color color, Point3f _lightPos) const;
    void drawGrid(Color color = Color()) const;

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
