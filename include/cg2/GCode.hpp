#pragma once
#include <vector>

namespace cg2 
{
  class GCodeInstruction
  {
    virtual string asString() = 0;
  };

  vector<GCodeInstruction*> GCodeInstructions;

  class GCode : public GCodeInstructions
  {
  };
}


