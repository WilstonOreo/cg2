#pragma once

#include "cg2/Plane.hpp"
#include "cg2/Compound.hpp"
#include "cg2/Triangle.hpp"

namespace cg2
{
  namespace mesh_detail 
  {
    /** @brief Base class for meshes *
     * @tparam TRIANGLE Triangle type
     */
    template<class TRIANGLE>
    struct MeshConcept : public Compound<TRIANGLE>
    {
      /// KDTree node type 
      typedef KDNode<TRIANGLE> Node;
      
      /** @brief Load mesh from file (virtual) 
       */
      virtual void read(const std::string& _filename) = 0;

      /** @brief return triangles **/
      const std::vector<TRIANGLE>& triangles() const { return Compound<TRIANGLE>::objs_; }
      std::vector<TRIANGLE>& triangles() { return Compound<TRIANGLE>::objs_; }

      bool intersect(Ray& _ray, Vec3f* _normal = NULL, Point2f* _texCoords = NULL) const
      {
        bool found = false;
        recKDTreeTraverse(_ray,this->root_,_ray.tMin_,_ray.tMax_,found,_normal,_texCoords);
        return found;
      }

    private:

      /** @brief Method to define how KDTree is constructed
       *  @param _node  Node to be divided
       *  @param _box   Node's BoundingBox
       *  @param _depth Node depth
       */
      void divideNode(Node* node, const BoundingBox& box, int depth)
      {
        if (depth > 15 || node->objs.size() < 10)
        {   // We have a leaf node!
          node->left = NULL; node->right = NULL;
          return;
        }
        node->left = new Node;
        node->right = new Node;
        node->axis = box.dominantAxis();

        // TODO: Surface Area Heuristic here!
        node->splitPos = 0.5*(box.min()[node->axis] + box.max()[node->axis]);
        BoundingBox boxLeft, boxRight;
        box.split(node->splitPos,node->axis,boxLeft,boxRight);

        BOOST_FOREACH( TRIANGLE* tri, node->objs )
        {
          int result = tri->intersect(node->splitPos,node->axis);
          if (result & 1) node->left->objs.push_back(tri);
          if (result & 2) node->right->objs.push_back(tri);
        }
        node->objs.clear();
        divideNode(node->left,boxLeft,depth+1);
        divideNode(node->right,boxRight,depth+1);
      }

      /** @brief Traverses kd-tree along a ray recursively
       * @param _ray        Ray which traverses KDTree
       * @param _node       Current node
       * @param _tnear      Near ray section
       * @param _tfar       Far ray section
       * @param _found      Flag which determines if a ray was found
       * @param _normal     Normal of intersecting triangle
       * @param _texCoords  Texture coordinates of intersecting triangle
       */
      float recKDTreeTraverse(Ray& ray, Node* node, float tnear, float tfar, bool& found,
          Vec3f* _normal = NULL, Point2f* _texCoords = NULL) const
      {
        if (node->isLeaf())
        {
          BOOST_FOREACH( TRIANGLE* tri, node->objs )
            if (tri != ray.primitive_) found |= (tri->intersect(ray,_normal,_texCoords));
          return ray.tMax_;
        }

        int k = node->axis;
        float d = (node->splitPos - ray.org_[k]) / ray.dir_[k];

        KDNode<TRIANGLE>* front = node->left;
        KDNode<TRIANGLE>* back  = node->right;
        if (ray.dir_[k] < 0) std::swap(front,back); 

        if (d <= tnear)
        {
          recKDTreeTraverse(ray,back,tnear,tfar,found);
        } else
          if (d >= tfar)
          {
            recKDTreeTraverse(ray,front,tnear,tfar,found);
          } else
          {
            float t_hit = recKDTreeTraverse(ray,front,tnear,d,found);
            if (t_hit <= d) return t_hit;
            return recKDTreeTraverse(ray,back,d,tfar,found,_normal,_texCoords);
          }
        return INF;
      }

    };
  }

  /** @brief Vertex mesh which consists of VertexTriangles and a vertex list 
   */
  struct VertexMesh : public mesh_detail::MeshConcept<VertexTriangle>
  {
    void read(const std::string& filename) {}
    std::vector<Vertex> vertices_;
    void draw(const Color4f& _color = Color4f()) const {} // TODO
  };

  /** @brief Mesh which consists of individual triangles 
   */
  struct TriangleMesh : public mesh_detail::MeshConcept<Triangle>
  {
    void read(const std::string& filename);

    /** @brief Split a mesh into halves along a split plane
     * @param _plane  Split plane
     * @returns Two new TriangleMeshes which represent halves of the original mesh 
     */
    std::pair<TriangleMesh,TriangleMesh> split(const Plane& plane);
    void draw(const Color4f& _color = Color4f()) const;

    private:

    /** @brief Splits are triangle with splitting plane
     * @detail Adds triangles behind plane to behind and triangles in front of plane to front
     * @param _tri    Triangle to be split
     * @param _plane  Split plane
     * @param _halves Halves of the mesh where the resulting triangles are inserted 
     */
    void splitTriangle(const Triangle& tri, const Plane& plane, std::pair<TriangleMesh,TriangleMesh>& _halves);

  };

}
