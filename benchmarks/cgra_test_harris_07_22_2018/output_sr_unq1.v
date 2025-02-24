//
//--------------------------------------------------------------------------------
//          THIS FILE WAS AUTOMATICALLY GENERATED BY THE GENESIS2 ENGINE        
//  FOR MORE INFORMATION: OFER SHACHAM (CHIP GENESIS INC / STANFORD VLSI GROUP)
//    !! THIS VERSION OF GENESIS2 IS NOT FOR ANY COMMERCIAL USE !!
//     FOR COMMERCIAL LICENSE CONTACT SHACHAM@ALUMNI.STANFORD.EDU
//--------------------------------------------------------------------------------
//
//  
//	-----------------------------------------------
//	|            Genesis Release Info             |
//	|  $Change: 11904 $ --- $Date: 2013/08/03 $   |
//	-----------------------------------------------
//	
//
//  Source file: /horowitz/users/dhuff/CGRAGenerator/hardware/generator_z/memory_core/output_sr.vp
//  Source template: output_sr
//
// --------------- Begin Pre-Generation Parameters Status Report ---------------
//
//	From 'generate' statement (priority=5):
// Parameter dwidth 	= 16
//
//		---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----
//
//	From Command Line input (priority=4):
//
//		---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----
//
//	From XML input (priority=3):
//
//		---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----
//
//	From Config File input (priority=2):
//
// ---------------- End Pre-Generation Pramameters Status Report ----------------

///////////////////////////////////////////////////////////////////
// CGRA memory generator
//////////////////////////////////////////////////////////////////


`define xassert(condition, message)
//if(condition) begin $display(message); $finish(1); end


// dwidth (_GENESIS2_INHERITANCE_PRIORITY_) = 16
//
module output_sr_unq1(
clk,
reset,
flush,
fsm_en,
data_in,
wen_w1,
wen_w2,
data_out,
avail,
ren,
valid
);

  input clk;
  input reset;
  input flush;
  input fsm_en;
  input [31:0] data_in;
  input wen_w2;
  input wen_w1;
  input ren;
  output reg valid;
  output reg avail;
  output reg [15:0] data_out;
  //3x data bits + 3 valid bits
  reg [47:0] data_sr;
  reg [1:0] count;
  

  always @(posedge clk) begin
   // assert only wen_w2 or wen_w1 at a time.
   // asserting both is an error
   `xassert((wen_w2&wen_w1)==1'b1 , "both wen_w2 and wen_w1 are asserted" )
   if (fsm_en==1'b1) begin
     if (wen_w2==1'b1) begin
       data_sr <= {data_sr[15:0],data_in};
     end else if (wen_w1==1'b1) begin
       data_sr <= {data_sr[31:0],data_in[15:0]};
     end
   end
  end

  always @(posedge clk or posedge reset) begin
    if (reset==1'b1) begin
      count <= 2'd0;
	end
    else if (flush==1'b1) begin
      count <= 2'd0;
    end 
	else if (fsm_en==1'b1) begin
      if (((wen_w1==1'b0) && (wen_w2==1'b0)) && ren==1'b1) begin
        count <= count - 2'd1;
        `xassert(count == 2'd0, "popping 1 pixel from output sr when count = 0")
      end else if (wen_w1==1'b1 && ren==1'b0) begin
        count <= count + 2'd1;
        `xassert(count >= 2'd3, "pushing 1 pixel into output sr when count >= 3")
      end else if (wen_w2==1'b1 && ren==1'b0) begin
        count <= count + 2'd2;
        `xassert(count >= 2'd2, "pushing 2 pixels into output sr when count >= 2")
      end else if (wen_w1==1'b1 && ren==1'b1) begin
        count <= count;
      end else if (wen_w2==1'b1 && ren==1'b1) begin
        count <= count + 2'd1;
        `xassert(count >= 2'd3, "pushing 2 pixels into output sr when count >= 3")
      end
    end
  end


  always @(*) begin
      avail <= 1'b0;
      if (((wen_w1==1'b0)&&(wen_w2==1'b0)) && ren==1'b0) begin
        avail <= (count >= 2'd1);
      end else if (((wen_w1==1'b0)&&(wen_w2==1'b0)) && ren==1'b1) begin
        avail <= (count >= 2'd2);
      end else if ((wen_w1==1'b1||wen_w2==1'b1) && ren==1'b0) begin
        avail <= 1'b1;
      end else if ((wen_w1==1'b1||wen_w2==1'b1) && ren==1'b1) begin
        avail <= 1'b1;
      end
  end

  always @(*) begin
    if (ren==1'b1) begin
      if (count==2'd1) begin
        data_out <= {data_sr[15:0]};
      end else if (count==2'd2) begin
        data_out <= {data_sr[31:16]};
      end else if (count==2'd3) begin
        data_out <= {data_sr[47: 32]};
      end else begin
        data_out <= 16'h0;
      end
    end else begin
      data_out <= 16'd0;
    end
  end
  always @(*) begin
    if ((ren==1'b1) && (count>=2'd1)) begin
      valid <= 1'b1;
    end else begin
      valid <= 1'b0;
    end
  end
endmodule

