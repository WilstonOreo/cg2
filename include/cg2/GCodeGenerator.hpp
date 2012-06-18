#pragma once
#include <tbd/config.hpp>

namespace cg2
{
  class GCodeGenerator : public ConfigurableObject
  {
    GCodeGenerator(Config * _config = NULL) : ConfigurableObject(_config) {};

    GCode generate(const Slices & slices const PrinterSetup & printerSetup);

  };
}
