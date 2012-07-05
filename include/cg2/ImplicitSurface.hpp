
#include "cg2/PointCloud.hpp"
#include "cg2/Mesh.hpp"

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
    float size() const { return (f_[7].point_ - f_[0].point_).length(); }

    ImplicitSample& sample(int _x, int _y, int _z);
    const ImplicitSample& sample(int _x, int _y, int _z) const;
    void draw(const Color4f& color = Color4f()) const;

  private:
    ImplicitSample f_[8];
  };
 
  struct VoxelTreeNode
  {
    VoxelTreeNode(Axis _axis = X, float _splitPos = 0)
    {
      axis_ = _axis;
      splitPos_ = _splitPos;
      left_ = NULL; right_ = NULL;
    }

    bool isLeaf() const { return (!left_ && !right_); }

    VoxelTreeNode* left_;
    VoxelTreeNode* right_;

    Axis axis_;
    float splitPos_;
    unsigned int voxelIdx_;

    void free() 
    { 
      left_->free(); 
      delete left_; 
      left_ = NULL; 
      right_->free();
      delete right_;
      right_ = NULL;
    }

/*    void draw(Color color, const BoundingBox& box, int depth, int maxDepth) const
    {
      if (isLeaf() || depth >= maxDepth) { box.draw(color); return; }

      BoundingBox boxLeft, boxRight;
      box.split(splitPos,axis,boxLeft,boxRight);
      if (left) left->draw(color,boxLeft,depth+1,maxDepth);
      if (right) right->draw(color,boxRight,depth+1,maxDepth);
    }
    */
  };

  class VoxelKDTree
  {
    public:
    VoxelKDTree() : root_(NULL) {}
    typedef VoxelTreeNode Node;
    Node* root_;
    void clear()
    {
      if (!root_) return;
      root_->free();
      delete root_;
      root_ = NULL;
    }

    const Node* root() const { return root_; }
    
    vector<Voxel> voxels_;

    /// Get a voxel from a point
    const Voxel* voxel(const Point3f& _p, const Node* _node, const Bounds& _bounds ) const;

     void build(PointCloud& _pointCloud, const Bounds& _bounds)
    {
      /*
      clear();
      root_ = new Node;
      root_->objs.reserve(objs.size());
      
      for (unsigned i = 0; i < objs.size(); i++)
        root_->objs.push_back(&objs[i]);
      
      divideNode(root_,boundingBox,0);*/
    }

    virtual void divideNode(Node* node, const BoundingBox& box, int depth);
  };


  class ImplicitSurface
  {
  public:
    void make(PointCloud& _pointCloud);
    Mesh polygonize() const;
    
    void evaluate(const Point3f& _p, float* _f = NULL, Vec3f* _gradient = NULL) const;
    Point3f project(const Point3f& _p, float* _f = NULL, Vec3f* _gradient = NULL) const;

    void read(string filename);

    void draw(const Color4f& color = Color4f()) const;
    void drawPoints(const Color4f& color, Point3f _lightPos) const;
    void drawGrid(const Color4f& color = Color4f()) const;

  private:


    Voxel getVoxel(const Point3f& _p);

    void marchingCube(const Voxel& _voxel, Mesh& _mesh) const;
  Vertex sampleInterp(const ImplicitSample& _a, const ImplicitSample& _b) const;

    VoxelKDTree kdTree_;

    float epsilon_;

    TBD_PROPERTY_REF(Bounds,bounds);
  };
}
