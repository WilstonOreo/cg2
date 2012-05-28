#include "cg2/Slicer.hpp"

namespace cg2 {
	Slices Slicer::slice(SceneObject & obj, unsigned nSlices) {

		Slices slices;
		slices.resize(nSlices);
		/*    for (unsigned i = 0; i < nSlices; i++)
		    {
		      slices.push_back(generateSlice(obj,slicePos,axis,i,resX,resY));
		      slicePos += sliceThickness;
		    }*/
		return slices;
	}

	Slice Slicer::generateSlice(SceneObject & obj, unsigned sliceIdx) {
		return Slice();
	}
	/*    Slice slice;
	    vector<Traces> traces(4);

	    Ray ray;
	    for (unsigned x = 0; x < resX; x++)
	    {
	      ray = borderRay(obj.boundingBox,posX,posY,DIR_LEFT);
	      traces[DIR_LEFT].push_back(trace(ray,obj,DIR_LEFT));
	      ray = borderRay(obj.boundingBox,posX,posY,DIR_RIGHT);
	      traces[DIR_RIGHT].push_back(trace(ray,obj,DIR_RIGHT));
	    }

	    for (unsigned y = 0; x < resY; x++)
	    {
	      ray = borderRay(obj.boundingBox,posX,posY,DIR_FRONT);
	      traces[DIR_FRONT].push_back(trace(ray,obj,DIR_FRONT));
	      ray = borderRay(obj.boundingBox,posX,posY,DIR_BACK);
	      traces[DIR_BACK].push_back(trace(ray,obj,DIR_BACK));
	    }

	    return mergeTraces(traces);
	  }

	  Trace Slicer::trace(Ray& ray, IntersectableObject& obj, Direction direction)
	  {
	    Trace trace(direction);
	    bool inside = false;

	    while (obj.intersect(ray))
	    {
	      Bounce bounce(ray.getIntersectionPoint(),inside);
	      trace.push_back(bounce);
	      inside = !inside;
	    }
	    return trace;
	  }

	  Ray Slicer::borderRay(BoundingBox& box, unsigned resX, unsigned resY, Direction direction)
	  {

	  }

	  Slice Slicer::mergeTraces(vector<Traces>& traces)
	  {

	  }*/
}
