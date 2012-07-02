
#include "cg2/PointCloud.hpp"
#include "cg2/Mesh.hpp"
#include "cg2/Image.hpp"

namespace cg2
{
  /// Point in implicite surface which has normal and function value
  struct ImplicitSample
  {
    Point3f point_;
    Vec3f normal_;
    float f_;
  };

  struct Voxel
  {
  public:

    ImplicitSample& sample(int _x, int _y, int _z);
    const ImplicitSample& sample(int _x, int _y, int _z) const;

  private:
    ImplicitSample f_[8];
  };
  
  class VoxelKDTree : KDTree<Vertex>
  {
    typedef KDNode<Vertex> Node;

    /// Get a voxel from a point
    Voxel voxel(const Point& _p, const Node* _node, const Bounds& _bounds ) const
    {
      if (_node->isLeaf())
      {
        return _node->voxel_;
      }

      Bounds _left, _right;
      _bounds.split(_node->axis,_node->splitPos,_left,_right);

      if (_p[_node->axis] < _node->splitPos)
        return voxel(_p,_node->left,_left);
      else
        return voxel(_p,_node->right,_right);
    }


    virtual void divideNode(Node* node, const BoundingBox& box, int depth);
  };



  class ImplicitSurface
  {
  public:
    void make(const PointCloud& _pointCloud);
    Mesh polygonize() const;
    
    void evaluate(const Point3f& _p, float* _f = NULL, Vec3f* _gradient = NULL);
    Point3f project(const Point3f& _p, float* _f = NULL, Vec3f* _gradient = NULL);

    void read(string filename);

    void draw(const Color4f& color = Color4f()) const;
    void drawPoints(const Color4f& color, Point3f _lightPos) const;
    void drawGrid(const Color4f& color = Color4f()) const;

  protected:
    void calcBoundingBox(); 

  private:

    Voxel getVoxel(const Point3f& _p);

    void marchingCube(const Bounds& _bounds);
    Vertex vertexInterp(Voxel* _a, Voxel* _b);

    VoxelKDTree kdTree_;
  };
}
