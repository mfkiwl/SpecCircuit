#pragma once

#include "circuit.h"

namespace FlatCircuit {

  void outputVerilog(const CellDefinition& def,
                     const std::string& verilogFile);

}
