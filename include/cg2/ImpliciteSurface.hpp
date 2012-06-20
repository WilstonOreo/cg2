
#include "cg2/PointCloud.hpp"
#include "cg2/Mesh.hpp"
#include "cg2/Image.hpp"

namespace cg2
{
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

    Voxel* voxel(unsigned _posX, unsigned _posY, unsigned _posZ)
    {
      return &voxels_[ (( y_ * _posZ ) + _posY)  * x_ + _posX];
    }

    const Voxel* voxel(unsigned _posX, unsigned _posY, unsigned _posZ) const
    {
      return &voxels_[ (( y_ * _posZ ) + _posY)  * x_ + _posX];
    }

    const Voxel* voxel(const Point3f& _point) const
    {
      Vec3f invSize(1.0f/boundingBox_.size().x()*x_,
                    1.0f/boundingBox_.size().y()*y_,
                    1.0f/boundingBox_.size().z()*z_);
      Vec3f _org =  (_point - boundingBox_.min());
      Vec3f _p = _org * invSize;

      int _x = int(_p.x()), _y = int(_p.y()), _z = int(_p.z());

      if (_x < 0 || _x >= x_ || _y < 0 || _y >= y_ || _z < 0 || _z >= z_)
        return NULL;

      return voxel(_x,_y,_z);
    }

    float value(const Point3f& _point) const
    {
      Vec3f _sizePos = 0.5*voxelSize(), _sizeNeg = -_sizePos;
      BoundingBox box;
      box.min(_point + _sizeNeg);
      box.max(_point + _sizePos);
      return 0.0;
//      Voxel* v000 = voxel(Point
    }

    Vec3f voxelSize() const;

    void draw(const Color4f& color = Color4f()) const;
    void drawPoints(const Color4f& color, Point3f _lightPos) const;
    void drawValues(const Color4f& color, Point3f _lightPos) const;
    void drawGrid(const Color4f& color = Color4f()) const;

    bool intersect(Ray& _ray, Vec3f* _normal = NULL, Point2f* _texCoords = NULL) const;

  protected:
    void calcBoundingBox();
    void calcBorderConditions(float _epsilon, std::vector<float>& _f);

  private:
    void calcImplicit();
    void marchingCubes();
    Vertex vertexInterp(Voxel* _a, Voxel* _b);
    bool gridTraversal(Ray& ray, Vec3f& _normal) const;

    int x_, y_, z_;

    std::vector<Voxel> voxels_;
    std::vector<float> f_pN_, f_p2N_;

    VertexMesh mesh_;

    float epsilon_;
  };
}
