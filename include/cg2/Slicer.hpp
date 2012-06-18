#include "cg2/Slice.hpp"
#include "cg2/SceneObject.hpp"

#include <tbd/config.h>

namespace cg2
{
  using tbd::Config;
  using tbd::ConfigurableObject;

  class Slicer : public ConfigurableObject
  {
  public:
    Slicer(Config * _config = NULL) : ConfigurableObject(_config) {}

    Slices slice(SceneObject & obj, unsigned nSlices = 0);

    TBD_PROPERTY_CFG(unsigned,nSlices,"NUMBER_OF_SLICES",10);
    TBD_PROPERTY_CFG(unsigned,resX,"RESOLUTION_X",1000);
    TBD_PROPERTY_CFG(unsigned,resY,"RESOLUTION_Y",1000);

  private:
    Slice generateSlice(SceneObject & obj, unsigned sliceIdx);
  };
}


