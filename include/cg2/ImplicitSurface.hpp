
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

    float length() const { return (f_[7].point_ - f[0].point_).length(); }

    ImplicitSample& sample(int _x, int _y, int _z);
    const ImplicitSample& sample(int _x, int _y, int _z) const;

  private:
    ImplicitSample f_[8];
  };
 
  struct VoxelTreeNode
  {
    KDNode(Axis _axis = X, float _splitPos = 0)
    {
      axis = _axis;
      splitPos = _splitPos;
      left = NULL; right = NULL;
    }

    bool isLeaf() const { return (!left && !right); }

    VoxelTreeNode* left_;
    VoxelTreeNode* right_;

    Axis axis_;
    float splitPos_;
    Voxel* voxel_;

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

  template <typename T> 
  struct KDTree
  {
    KDTree() : root_(NULL) {}

  protected:
    typedef KDNode<T> Node;
    Node* root_;
    void clear()
    {
      if (!root_) return;
      root_->free();
      delete root_;
      root_ = NULL;
    }

/*  void draw(Color color, const BoundingBox& box) const
    {
      if (root) root->draw(color,box,0,12);
    }
*/

     void build(std::vector<T>& objs, const BoundingBox& boundingBox)
    {
      clear();
      root_ = new Node;
      root_->objs.reserve(objs.size());
      
      for (unsigned i = 0; i < objs.size(); i++)
        root_->objs.push_back(&objs[i]);
      
      divideNode(root_,boundingBox,0);
    }

   virtual void divideNode(Node* node, const BoundingBox& boundingBox, int depth) = 0;
  };


  class VoxelKDTree
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

    void marchingCube(const Voxel& _voxel, Mesh& _mesh) const;
  Vertex sampleInterp(const ImplicitSample& _a, const ImplicitSample& _b) const;

    VoxelKDTree kdTree_;
    vector<Voxel> voxels_;
  };
}
