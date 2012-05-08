#pragma once

namespace cg2 
{
  struct KDNode
  {
    KDNode(int _axis = 0, float _splitPos = 0)
    {
      axis = _axis;
      splitPos = _splitPos;
      left = NULL; right = NULL;
    }

    bool isLeaf() { return (left == NULL && right == NULL); }

    KDNode* left;
    KDNode* right;
    int axis;
    float splitPos;
    vector<Vertex*> tris;
  };

  struct KDTree : public IntersectableObject
  {
    KDNode* root;
  };
}

