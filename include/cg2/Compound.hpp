#pragma once

#include "cg2/Primitive.hpp"
#include "cg2/BoundingBox.hpp"
#include "cg2/KDTree.hpp"

#include <boost/foreach.hpp>

namespace cg2
{

  /** @brief A Compound is object is a primitive which consists of several primtives
   * @detail Moreover, a compound holds a KDTree structure for fast intersection + traversal
   * @tparam PRIMITIVE  Primitive type
   */
  template <class PRIMITIVE>
  class Compound : public Primitive, public KDTree<PRIMITIVE>
  {
    public:
      /* @brief Return bounds of bounding box
       */
      Bounds bounds() const
      {
        return Bounds(boundingBox_.min(),boundingBox_.max());
      }

      std::vector<PRIMITIVE>& objs() { return Compound<PRIMITIVE>::objs_; }
      const std::vector<PRIMITIVE>& objs() const { return Compound<PRIMITIVE>::objs_; }


    protected:
      /* @brief Determine extents of bounding box
       */
      virtual void calcBoundingBox()
      {
        boundingBox_ = BoundingBox();
        BOOST_FOREACH ( PRIMITIVE& _obj , objs_ ) 
          boundingBox_.extend(_obj.bounds());
        boundingBox_.validate();
      }

      /// Bounds are caches by a bounding box
      BoundingBox boundingBox_;
      
      /// Container for primitives
      std::vector<PRIMITIVE> objs_;
  };
}


