#include "output_verilog.h"

#include <fstream>

using namespace std;

namespace FlatCircuit {

  std::string verilogCellName(const CellId cid, const CellDefinition& def) {
    string str = def.getCellName(cid);
    if (!def.isPortCell(cid)) {
      return str;
    }

    return str + "_port_cell";
  }
  
  std::string verilogName(const CellType tp) {
    return toString(tp);
  }


  class VerilogWire {
  public:

    std::string name;
    int width;
    bool isInput;
    bool isOutput;
    bool isRegister;

    VerilogWire(const std::string& name_, const int width_) :
      name(name_), width(width_),
      isInput(false), isOutput(false), isRegister(false) {}

    VerilogWire(const std::string& name_, const int width_,
                const bool isIn_, const bool isOut_, const bool isReg_) :
      name(name_), width(width_),
      isInput(isIn_), isOutput(isOut_), isRegister(isReg_) {}
    
  };

  class VerilogInstance {
  public:
    std::string modName;
    std::string instName;

    std::map<string, string> parameters;
    std::map<string, string> portsToWires;

    VerilogInstance(const std::string& modName_,
                    const std::string& instName_) :
      modName(modName_), instName(instName_) {}

    std::string toString() const {
      string str = "\t" + modName;

      if (parameters.size() > 0) {
        str += " #(";

        int i = 0;
        for (auto pm : parameters) {
          str += "." + pm.first + "(" + pm.second + ")";
          if (i != (int) (parameters.size() - 1)) {
            str += ",\n";
          }
          i++;
        }

        str += ")";
      }

      str += + " " + instName;
      str += "(";

      int i = 0;
      for (auto pw : portsToWires) {
        str += "." + pw.first + "(" + pw.second + ")";
        if (i != (int) (portsToWires.size() - 1)) {
          str += ",\n";
        }
        i++;
      }

      str += ");\n";

      return str;
    }

    void connect(const std::string& port, const std::string& wire) {
      portsToWires.insert({port, wire});
    }
  };

  class VerilogConnection {
  };

  class VerilogAssign {
  public:
    VerilogWire receiverWire;
    int receiverBit;
    VerilogWire driverWire;
    int driverBit;

    VerilogAssign(const VerilogWire receiverWire_,
                  const int receiverBit_,
                  const VerilogWire driverWire_,
                  const int driverBit_) : receiverWire(receiverWire_),
                                          receiverBit(receiverBit_),
                                          driverWire(driverWire_),
                                          driverBit(driverBit_) {}

  };
  
  class VerilogModule {
    unsigned long long nextInt;
  public:

    std::string name;

    std::map<std::string, VerilogInstance> instances;
    std::vector<VerilogConnection> connections;

    std::vector<VerilogWire> wires;
    std::vector<VerilogAssign> assigns;

    VerilogModule() : nextInt(0) {}

    VerilogInstance& getInstance(const std::string& str) {
      if (!contains_key(str, instances)) {
        cout << "Verilog module does not contain " << str << endl;
      }
      assert(contains_key(str, instances));
      
      return instances.at(str);
    }

    VerilogWire freshWire(const int width) {
      std::string name = "fresh_wire_" + to_string(nextInt);
      nextInt++;

      VerilogWire w(name, width);
      wires.push_back(w);

      return w;
    }

    void addOutput(const std::string& name, const int width, const CellId cid, const CellDefinition& def) {
      wires.push_back(VerilogWire(name, width, false, true, false));

      CellType tp = def.getCellRefConst(cid).getCellType();
      instances.insert({verilogCellName(cid, def),
            VerilogInstance(verilogName(tp), verilogCellName(cid, def))});
    }

    void addInput(const std::string& name, const int width, const CellId cid, const CellDefinition& def) {
      wires.push_back(VerilogWire(name, width, true, false, false));

      CellType tp = def.getCellRefConst(cid).getCellType();
      instances.insert({verilogCellName(cid, def),
            VerilogInstance(verilogName(tp), verilogCellName(cid, def))});
      
    }

    void addBitAssign(const VerilogWire receiverWire,
                      const int receiverBit,
                      const VerilogWire driverWire,
                      const int driverBit) {
      assigns.push_back(VerilogAssign(receiverWire, receiverBit,
                                      driverWire, driverBit));
    }

    std::string toString() const {
      string str = "module " + name + "(";

      vector<VerilogWire> ports;
      for (auto w : wires) {
        if (w.isInput || w.isOutput) {
          ports.push_back(w);
        }
      }

      int i = 0;
      for (auto w : ports) {
        if (w.isInput) {
          str += "\tinput [" + to_string(w.width - 1) + " : 0] " + w.name;
        } else if (w.isOutput) {
          str += "\toutput [" + to_string(w.width - 1) + " : 0] " + w.name;
        } else {
          assert(false);
        }

        if (i != (int) (ports.size() - 1)) {
          str += ",\n";
        }

        i++;
      }
      
      str += ");\n\n";

      for (auto w : wires) {
        if (!w.isInput && !w.isOutput) {
          str += "\twire [" + to_string(w.width - 1) + " : 0] " + w.name + ";\n";
        }
      }

      for (auto inst : instances) {
        str += inst.second.toString();
        str += "\n";
      }

      for (auto assign : assigns) {
        str += "\tassign " + assign.receiverWire.name +
          "[ " + to_string(assign.receiverBit) + " ] = " +
          assign.driverWire.name + "[ " + to_string(assign.driverBit) + " ];";
        str += "\n";
      }

      str += "endmodule\n";
      return str;
    }

    void addUnop(const CellId cid,
                 const CellDefinition& def) {
      CellType tp = def.getCellRefConst(cid).getCellType();
      instances.insert({verilogCellName(cid, def),
            VerilogInstance(verilogName(tp), verilogCellName(cid, def))});
    }

    void addBinop(const CellId cid,
                  const CellDefinition& def) {
      CellType tp = def.getCellRefConst(cid).getCellType();
      instances.insert({verilogCellName(cid, def),
            VerilogInstance(verilogName(tp), verilogCellName(cid, def))});
    }

  };

  void cellToVerilogInstance(const CellId cid,
                             VerilogModule& vm,
                             const CellDefinition& def,
                             map<PortId, VerilogWire>& portMap) {
    const Cell& cell = def.getCellRefConst(cid);
    CellType tp = cell.getCellType();

    if (def.isPortCell(cid) && cell.isInputPortCell()) {
      vm.addInput(def.getCellName(cid), cell.getPortWidth(PORT_ID_OUT), cid, def);
    } else if (def.isPortCell(cid) && !cell.isInputPortCell()) {
      vm.addOutput(def.getCellName(cid), cell.getPortWidth(PORT_ID_IN), cid, def);
    } else if (isUnop(tp)) {
      vm.addUnop(cid, def);
    } else if (isBinop(tp)) {
      vm.addBinop(cid, def);
    } else if (tp == CELL_TYPE_CONST) {
      CellType tp = def.getCellRefConst(cid).getCellType();
      vm.instances.insert({verilogCellName(cid, def),
            VerilogInstance(verilogName(tp), verilogCellName(cid, def))});
    } else {
      CellType tp = def.getCellRefConst(cid).getCellType();
      vm.instances.insert({verilogCellName(cid, def),
            VerilogInstance(verilogName(tp), verilogCellName(cid, def))});
    }
    // else {
    //   cout << "Unsupported cell type for cell " << verilogCellName(cid, def) << endl;
    //   assert(false);
    // }

    VerilogInstance& inst = vm.getInstance(verilogCellName(cid, def));
    for (auto pm : portMap) {
      inst.connect(portIdString(pm.first), pm.second.name);
    }

  }

  VerilogModule toVerilog(const CellDefinition& def) {
    VerilogModule vm;
    vm.name = "top";

    map<string, map<PortId, VerilogWire> > instancePortWires;

    for (auto ctp : def.getCellMap()) {
      CellId cid = ctp.first;
      string cellName = verilogCellName(cid, def);

      map<PortId, VerilogWire> portWires;

      const Cell& cell = def.getCellRefConst(cid);

      for (auto ptp : cell.getPorts()) {
        PortId pid = ptp.first;
        VerilogWire w = vm.freshWire(cell.getPortWidth(pid));
        portWires.insert({pid, w});
      }

      if (def.isPortCell(cid)) {
        if (cell.isInputPortCell()) {
          portWires.insert({PORT_ID_IN,
                VerilogWire(def.getCellName(cid),
                            cell.getPortWidth(PORT_ID_OUT))});
        } else {
          portWires.insert({PORT_ID_OUT,
                VerilogWire(def.getCellName(cid),
                            cell.getPortWidth(PORT_ID_IN))});
        }
      }

      cellToVerilogInstance(cid, vm, def, portWires);

      VerilogInstance& m = vm.getInstance(verilogCellName(cid, def));
      for (auto pm : cell.getParameters()) {
        m.parameters.insert({parameterToString(pm.first),
              pm.second.hex_string()});
              //              to_string(bvToInt(pm.second))});
      }
      
      instancePortWires[cellName] = portWires;
    }

    cout << "Adding connections" << endl;
    for (auto ctp : def.getCellMap()) {
      CellId cid = ctp.first;
      const Cell& cell = def.getCellRefConst(cid);
      for (auto pid : cell.inputPorts()) {
        auto drivers = cell.getDrivers(pid);

        for (int i = 0; i < (int) drivers.size(); i++) {
          SignalBit driverBit = drivers.signals[i];
          SignalBit receiverBit = {cid, pid, i};

          VerilogWire& driverWire = instancePortWires.at(verilogCellName(driverBit.cell, def)).at(driverBit.port);
          VerilogWire& receiverWire = instancePortWires.at(verilogCellName(receiverBit.cell, def)).at(receiverBit.port);

          vm.addBitAssign(receiverWire, receiverBit.offset, driverWire, driverBit.offset);
        }
      }
    }

    return vm;
  }

  std::string verilogPrefix() {
    string str = "";

    str += "module CELL_TYPE_CONST #(parameter PARAM_WIDTH=1, parameter PARAM_INIT_VALUE=0) (output [PARAM_WIDTH - 1 : 0] PORT_ID_OUT); assign PORT_ID_OUT = PARAM_INIT_VALUE;\nendmodule\n\n";

    str += "module CELL_TYPE_MUL #(parameter PARAM_WIDTH=1) (input [PARAM_WIDTH - 1 : 0] PORT_ID_IN0, input [PARAM_WIDTH - 1 : 0] PORT_ID_IN1, output [PARAM_WIDTH - 1 : 0] PORT_ID_OUT); assign PORT_ID_OUT = PORT_ID_IN0 * PORT_ID_IN1;\n endmodule\n\n";

    str += "module CELL_TYPE_NOT #(parameter PARAM_WIDTH=1) (input [PARAM_WIDTH - 1 : 0] PORT_ID_IN, output [PARAM_WIDTH - 1 : 0] PORT_ID_OUT); assign PORT_ID_OUT = ~PORT_ID_IN;\n endmodule\n\n";

    str += "module CELL_TYPE_ADD #(parameter PARAM_WIDTH=1) (input [PARAM_WIDTH - 1 : 0] PORT_ID_IN0, input [PARAM_WIDTH - 1 : 0] PORT_ID_IN1, output [PARAM_WIDTH - 1 : 0] PORT_ID_OUT);\n"
      "   assign PORT_ID_OUT = PORT_ID_IN0 + PORT_ID_IN1;\n"
      " endmodule\n\n";

    str += "module CELL_TYPE_SUB #(parameter PARAM_WIDTH=1) (input [PARAM_WIDTH - 1 : 0] PORT_ID_IN0, input [PARAM_WIDTH - 1 : 0] PORT_ID_IN1, output [PARAM_WIDTH - 1 : 0] PORT_ID_OUT);"
      "   assign PORT_ID_OUT = PORT_ID_IN0 - PORT_ID_IN1;"
      " endmodule\n\n";

    str += "module CELL_TYPE_AND #(parameter PARAM_WIDTH=1) (input [PARAM_WIDTH - 1 : 0] PORT_ID_IN0, input [PARAM_WIDTH - 1 : 0] PORT_ID_IN1, output [PARAM_WIDTH - 1 : 0] PORT_ID_OUT);\n"
      "   assign PORT_ID_OUT = PORT_ID_IN0 & PORT_ID_IN1;\n"
      "endmodule\n\n";

    str += "module CELL_TYPE_LSHR #(parameter PARAM_WIDTH=1) (input [PARAM_WIDTH - 1 : 0] PORT_ID_IN0, input [PARAM_WIDTH - 1 : 0] PORT_ID_IN1, output [PARAM_WIDTH - 1 : 0] PORT_ID_OUT);\n"
      "   assign PORT_ID_OUT = PORT_ID_IN0 >> PORT_ID_IN1;\n"
      "endmodule\n\n";

    str += "module CELL_TYPE_ASHR #(parameter PARAM_WIDTH=1) (input [PARAM_WIDTH - 1 : 0] PORT_ID_IN0, input [PARAM_WIDTH - 1 : 0] PORT_ID_IN1, output [PARAM_WIDTH - 1 : 0] PORT_ID_OUT);\n"
      "   assign PORT_ID_OUT = PORT_ID_IN0 >>> PORT_ID_IN1;\n"
      "endmodule\n\n";
    
    str += "module CELL_TYPE_XOR #(parameter PARAM_WIDTH=1) (input [PARAM_WIDTH - 1 : 0] PORT_ID_IN0, input [PARAM_WIDTH - 1 : 0] PORT_ID_IN1, output [PARAM_WIDTH - 1 : 0] PORT_ID_OUT);\n"
      "   assign PORT_ID_OUT = PORT_ID_IN0 ^ PORT_ID_IN1;\n"
      "endmodule\n\n";
    
    str += "module CELL_TYPE_OR #(parameter PARAM_WIDTH=1) (input [PARAM_WIDTH - 1 : 0] PORT_ID_IN0, input [PARAM_WIDTH - 1 : 0] PORT_ID_IN1, output [PARAM_WIDTH - 1 : 0] PORT_ID_OUT);\n"
      "   assign PORT_ID_OUT = PORT_ID_IN0 | PORT_ID_IN1;\n"
      "endmodule\n\n";

    str += "module CELL_TYPE_EQ #(parameter PARAM_WIDTH=1) (input [PARAM_WIDTH - 1 : 0] PORT_ID_IN0, input [PARAM_WIDTH - 1 : 0] PORT_ID_IN1, output [0 : 0] PORT_ID_OUT);\n"
      "   assign PORT_ID_OUT = PORT_ID_IN0 == PORT_ID_IN1;\n"
      "endmodule\n\n";

    str += "module CELL_TYPE_NEQ #(parameter PARAM_WIDTH=1) (input [PARAM_WIDTH - 1 : 0] PORT_ID_IN0, input [PARAM_WIDTH - 1 : 0] PORT_ID_IN1, output [0 : 0] PORT_ID_OUT);\n"
      "   assign PORT_ID_OUT = PORT_ID_IN0 != PORT_ID_IN1;\n"
      "endmodule\n\n";
    
    str += "module CELL_TYPE_ULT #(parameter PARAM_WIDTH=1) (input [PARAM_WIDTH - 1 : 0] PORT_ID_IN0, input [PARAM_WIDTH - 1 : 0] PORT_ID_IN1, output [0 : 0] PORT_ID_OUT); assign PORT_ID_OUT = PORT_ID_IN0 < PORT_ID_IN1;\n"
      " endmodule\n\n";

    str += "module CELL_TYPE_UGE #(parameter PARAM_WIDTH=1) (input [PARAM_WIDTH - 1 : 0] PORT_ID_IN0, input [PARAM_WIDTH - 1 : 0] PORT_ID_IN1, output [0 : 0] PORT_ID_OUT); assign PORT_ID_OUT = PORT_ID_IN0 >= PORT_ID_IN1;\n"
      "endmodule\n\n // CELL_TYPE_UGE\n";

    str += "module CELL_TYPE_REG #(parameter PARAM_CLK_POSEDGE=1, parameter PARAM_INIT_VALUE=0, parameter PARAM_WIDTH=1)(input PORT_ID_CLK, input [PARAM_WIDTH - 1 : 0] PORT_ID_IN, output [PARAM_WIDTH - 1 : 0] PORT_ID_OUT);\n"

      "   reg [PARAM_WIDTH - 1 : 0] data;\n"
   
      "   initial begin\n"
      "      data = PARAM_INIT_VALUE;\n"
      "   end\n"

      "   wire true_clk;\n"
      "   assign true_clk = PARAM_CLK_POSEDGE ? PORT_ID_CLK : ~PORT_ID_CLK;\n"
   
      "   always @(posedge true_clk) begin\n"
      "      data <= PORT_ID_IN;\n"
      "   end\n"

      "   assign PORT_ID_OUT = data;\n"
   
      "endmodule\n\n";

    str += "module CELL_TYPE_REG_ARST #(parameter PARAM_CLK_POSEDGE=1, parameter PARAM_ARST_POSEDGE=1, parameter PARAM_INIT_VALUE=0, parameter PARAM_WIDTH=1)(input PORT_ID_ARST, input PORT_ID_CLK, input [PARAM_WIDTH - 1 : 0] PORT_ID_IN, output [PARAM_WIDTH - 1 : 0] PORT_ID_OUT);\n"

      "   reg [PARAM_WIDTH - 1 : 0] data;\n"
   
      "   initial begin\n"
      "      data = PARAM_INIT_VALUE;\n"
      "   end\n"

      "   wire true_clk;\n"
      "   assign true_clk = PARAM_CLK_POSEDGE ? PORT_ID_CLK : ~PORT_ID_CLK;\n"

      "   wire true_rst;\n"
      "   assign true_rst = PARAM_ARST_POSEDGE ? PORT_ID_ARST : ~PORT_ID_ARST;\n"
   
      "   always @(posedge true_clk or posedge true_rst) begin\n"
      "      if (true_rst) begin\n"
      "         data <= PARAM_INIT_VALUE;\n"
      "      end else begin\n"
      "         data <= PORT_ID_IN;\n"
      "      end\n"
      "   end\n"

      "   assign PORT_ID_OUT = data;\n"
      //assign PORT_ID_OUT = 1;
   
   
      "endmodule\n\n // CELL_TYPE_REG_ARST\n";

    str += "module CELL_TYPE_MEM #(parameter PARAM_HAS_INIT=0, parameter PARAM_MEM_DEPTH=2, parameter PARAM_MEM_WIDTH=2)(input PORT_ID_CLK, input PORT_ID_WEN, input [$clog2(PARAM_MEM_DEPTH) - 1 : 0] PORT_ID_RADDR, input [$clog2(PARAM_MEM_DEPTH) - 1 : 0] PORT_ID_WADDR, input [PARAM_MEM_WIDTH - 1 : 0] PORT_ID_WDATA, output [PARAM_MEM_WIDTH - 1 : 0] PORT_ID_RDATA);\n"

      "   reg [PARAM_MEM_WIDTH -  1 : 0] data_array [0 : PARAM_MEM_DEPTH - 1];\n"

      "   always @(posedge PORT_ID_CLK) begin\n"
      "      if (PORT_ID_WEN) begin\n"
      "         data_array[PORT_ID_WADDR] = PORT_ID_WDATA;\n"
      "      end\n"
      "   end\n"
      "   assign PORT_ID_RDATA = data_array[PORT_ID_RADDR];\n"
      "endmodule\n\n";

    str += "module CELL_TYPE_MUX #(parameter PARAM_WIDTH=1) (input PORT_ID_SEL, input [PARAM_WIDTH - 1 : 0] PORT_ID_IN0, input [PARAM_WIDTH - 1 : 0] PORT_ID_IN1, output [PARAM_WIDTH - 1 : 0] PORT_ID_OUT);\n"
      "   assign PORT_ID_OUT = PORT_ID_SEL ? PORT_ID_IN1 : PORT_ID_IN0;\n"
      " endmodule\n\n";
    
    str += "module CELL_TYPE_ORR #(parameter PARAM_WIDTH=1) (input [PARAM_WIDTH - 1 : 0] PORT_ID_IN, output [0 : 0] PORT_ID_OUT); assign PORT_ID_OUT = |PORT_ID_IN;\n endmodule\n\n";

    str += "module CELL_TYPE_ANDR #(parameter PARAM_WIDTH=1) (input [PARAM_WIDTH - 1 : 0] PORT_ID_IN, output [0 : 0] PORT_ID_OUT); assign PORT_ID_OUT = &PORT_ID_IN;\n endmodule\n\n";
    
    str += "module CELL_TYPE_PORT #(parameter PARAM_PORT_TYPE=0, parameter PARAM_OUT_WIDTH=1) (input [PARAM_OUT_WIDTH - 1 : 0] PORT_ID_IN, output [PARAM_OUT_WIDTH - 1 : 0] PORT_ID_OUT); assign PORT_ID_OUT = PORT_ID_IN;\n endmodule\n\n";

    str += "module CELL_TYPE_ZEXT #(parameter PARAM_IN_WIDTH=1, parameter PARAM_OUT_WIDTH=1) (input [PARAM_IN_WIDTH - 1 : 0] PORT_ID_IN, output [PARAM_OUT_WIDTH - 1 : 0] PORT_ID_OUT); assign PORT_ID_OUT = {{(PARAM_OUT_WIDTH - PARAM_IN_WIDTH){1'b0}}, PORT_ID_IN};\n endmodule\n\n";

    str += "module CELL_TYPE_SLICE #(parameter PARAM_WIDTH=1, parameter PARAM_LOW=1, parameter PARAM_HIGH=1) (input [PARAM_WIDTH - 1 : 0] PORT_ID_IN, output [PARAM_HIGH - PARAM_LOW - 1 : 0] PORT_ID_OUT); assign PORT_ID_OUT = PORT_ID_IN[PARAM_HIGH:PARAM_LOW];\n endmodule\n\n";
    
    str += "module CELL_TYPE_PASSTHROUGH #(parameter PARAM_WIDTH=1) (input [PARAM_WIDTH - 1 : 0] PORT_ID_IN, output [PARAM_WIDTH - 1 : 0] PORT_ID_OUT); assign PORT_ID_OUT = PORT_ID_IN;\n endmodule\n\n";

    return str;

  }
  void outputVerilog(const CellDefinition& def,
                     const std::string& verilogFile) {

    VerilogModule mod = toVerilog(def);

    string verilogString = verilogPrefix();
    std::ofstream t(verilogFile);
    t << verilogString << endl;
    t << mod.toString() << endl;
    t.close();
    
  }
  
}
