// Full system test of configured CGRA verilog.
module test();

   reg [15:0]       data_driver_16_S2;

   reg clk;

   wire tdo;
   wire trst_n;
   wire tdi;
   wire tms;

   wire tck;

   reg [31:0] config_addr;
   reg [31:0] config_data;

   integer    config_file;
   integer    scan_file;
   integer    test_output_file;

   reg 	      config_done;

   reg [64:0] cycle_count;
   wire [64:0] max_cycles;

   assign max_cycles = 1000;

   initial begin

      cycle_count = 0;

      #1 clk = 0;

      data_driver_16_S2 = 3;

      test_output_file = $fopen("test_output.txt", "w");

      config_addr = 0;
      config_data = 0;
      config_done = 0;

   end // initial begin

   always #2 clk = ~clk;

   reg [0:0] data_in_S0_T0_reg;
   wire      data_in_S0_T0;

   wire [15:0] data_in_16_S2;
   wire [15:0] data_out_16_S0;

   // After reseting load data / configuration between rising clock edges
   always @(negedge clk) begin

      cycle_count <= cycle_count + 1;

      $fwrite(test_output_file, "%b\n", data_out_16_S0);
      

      if (cycle_count >= max_cycles) begin
	 $display("Finished at cycle count %d, data in = %b, %d, data out = %b, %d", cycle_count, data_in_16_S2, data_in_16_S2, data_out_16_S0, data_out_16_S0);
         $display("\tdata driver = %b, %d", data_driver_16_S2, data_driver_16_S2);

	 if (data_in_16_S2*2 != data_out_16_S0) begin
	    $display("Test FAILED, output is not 2x input!");
	 end

         $fclose(test_output_file);
//         $fclose(config_file);
	 $finish();
      end
//      end
   end

   
   wire [0:0] data_out_S0_T0;
   wire [0:0] data_out_S0_T1;
   wire [0:0] data_out_S0_T2;
   wire [0:0] data_out_S0_T3;
   wire [0:0] data_out_S0_T4;
   wire [0:0] data_out_S0_T5;
   wire [0:0] data_out_S0_T6;
   wire [0:0] data_out_S0_T7;
   wire [0:0] data_out_S0_T8;
   wire [0:0] data_out_S0_T9;
   wire [0:0] data_out_S0_T10;
   wire [0:0] data_out_S0_T11;
   wire [0:0] data_out_S0_T12;
   wire [0:0] data_out_S0_T13;
   wire [0:0] data_out_S0_T14;
   wire [0:0] data_out_S0_T15;

   assign data_out_16_S0 = {data_out_S0_T0,
			    data_out_S0_T1,
			    data_out_S0_T2,
			    data_out_S0_T3,
			    data_out_S0_T4,
			    data_out_S0_T5,
			    data_out_S0_T6,
			    data_out_S0_T7,
			    data_out_S0_T8,
			    data_out_S0_T9,
			    data_out_S0_T10,
			    data_out_S0_T11,
			    data_out_S0_T12,
			    data_out_S0_T13,
			    data_out_S0_T14,
			    data_out_S0_T15};
   
   
   
   always @(posedge clk) begin
//      $display("data_out_S0_T0 = %d", data_out_S0_T0);
//      $display("data_out_S0_T1 = %d", data_out_S0_T1);
//      $display("data_out_S0_T2 = %d", data_out_S0_T2);
//      $display("data_out_S0_T3 = %d", data_out_S0_T3);
//      $display("data_out_S0_T4 = %d", data_out_S0_T4);
//      $display("data_out_S0_T5 = %d", data_out_S0_T5);
//      $display("data_out_S0_T6 = %d", data_out_S0_T6);
//      $display("data_out_S0_T7 = %d", data_out_S0_T7);
//      $display("data_out_S0_T8 = %d", data_out_S0_T8);
//      $display("data_out_S0_T9 = %d", data_out_S0_T9);
//      $display("data_out_S0_T10 = %d", data_out_S0_T10);
//      $display("data_out_S0_T11 = %d", data_out_S0_T11);
//      $display("data_out_S0_T11 = %d", data_out_S0_T12);
//      $display("data_out_S0_T11 = %d", data_out_S0_T13);
//      $display("data_out_S0_T11 = %d", data_out_S0_T14);
//      $display("data_out_S0_T11 = %d", data_out_S0_T15);
   end

   
   wire [0:0] data_in_S2_T0;
   wire [0:0] data_in_S2_T1;
   wire [0:0] data_in_S2_T2;
   wire [0:0] data_in_S2_T3;
   wire [0:0] data_in_S2_T4;
   wire [0:0] data_in_S2_T5;
   wire [0:0] data_in_S2_T6;
   wire [0:0] data_in_S2_T7;
   wire [0:0] data_in_S2_T8;
   wire [0:0] data_in_S2_T9;
   wire [0:0] data_in_S2_T10;
   wire [0:0] data_in_S2_T11;
   wire [0:0] data_in_S2_T12;
   wire [0:0] data_in_S2_T13;
   wire [0:0] data_in_S2_T14;
   wire [0:0] data_in_S2_T15;   

   assign data_in_16_S2 = {
			   data_in_S2_T0,
			   data_in_S2_T1,
			   data_in_S2_T2,
			   data_in_S2_T3,
			   data_in_S2_T4,
			   data_in_S2_T5,
			   data_in_S2_T6,
			   data_in_S2_T7,
			   data_in_S2_T8,
			   data_in_S2_T9,
			   data_in_S2_T10,
			   data_in_S2_T11,
			   data_in_S2_T12,
			   data_in_S2_T13,
			   data_in_S2_T14,
			   data_in_S2_T15};

   always @(posedge clk) begin
      data_driver_16_S2 <= data_driver_16_S2;
   end

   assign    {data_in_S2_T0,
	      data_in_S2_T1,
	      data_in_S2_T2,
	      data_in_S2_T3,
	      data_in_S2_T4,
	      data_in_S2_T5,
	      data_in_S2_T6,
	      data_in_S2_T7,
	      data_in_S2_T8,
	      data_in_S2_T9,
	      data_in_S2_T10,
	      data_in_S2_T11,
	      data_in_S2_T12,
	      data_in_S2_T13,
	      data_in_S2_T14,
	      data_in_S2_T15} = data_driver_16_S2;

   assign data_in_S0_T0 = 0;
   assign data_in_S0_T1 = 0;
   assign data_in_S0_T2 = 0;
   assign data_in_S0_T3 = 0;
   assign data_in_S0_T4 = 0;
   assign data_in_S0_T5 = 0;
   assign data_in_S0_T6 = 0;
   assign data_in_S0_T7 = 0;
   assign data_in_S0_T8 = 0;
   assign data_in_S0_T9 = 0;
   assign data_in_S0_T10 = 0;
   assign data_in_S0_T11 = 0;
   assign data_in_S0_T12 = 0;
   assign data_in_S0_T13 = 0;
   assign data_in_S0_T14 = 0;
   assign data_in_S0_T15 = 0;
   
   top cgra(.clk_in(clk),
	    .reset_in(rst),
	    .config_addr_in(config_addr),
	    .config_data_in(config_data),

	    .pad_S0_T0_in(data_in_S0_T0),
	    .pad_S0_T1_in(data_in_S0_T0),
	    .pad_S0_T2_in(data_in_S0_T0),
	    .pad_S0_T3_in(data_in_S0_T0),
	    .pad_S0_T4_in(data_in_S0_T0),
	    .pad_S0_T5_in(data_in_S0_T0),
	    .pad_S0_T6_in(data_in_S0_T0),
	    .pad_S0_T7_in(data_in_S0_T0),
	    .pad_S0_T8_in(data_in_S0_T0),
	    .pad_S0_T9_in(data_in_S0_T0),
	    .pad_S0_T10_in(data_in_S0_T0),
	    .pad_S0_T11_in(data_in_S0_T0),
	    .pad_S0_T12_in(data_in_S0_T0),
	    .pad_S0_T13_in(data_in_S0_T0),
	    .pad_S0_T14_in(data_in_S0_T0),
	    .pad_S0_T15_in(data_in_S0_T0),

	    .pad_S1_T0_in(data_in_S0_T0),
	    .pad_S1_T1_in(data_in_S0_T0),
	    .pad_S1_T2_in(data_in_S0_T0),
	    .pad_S1_T3_in(data_in_S0_T0),
	    .pad_S1_T4_in(data_in_S0_T0),
	    .pad_S1_T5_in(data_in_S0_T0),
	    .pad_S1_T6_in(data_in_S0_T0),
	    .pad_S1_T7_in(data_in_S0_T0),
	    .pad_S1_T8_in(data_in_S0_T0),
	    .pad_S1_T9_in(data_in_S0_T0),
	    .pad_S1_T10_in(data_in_S0_T0),
	    .pad_S1_T11_in(data_in_S0_T0),
	    .pad_S1_T12_in(data_in_S0_T0),
	    .pad_S1_T13_in(data_in_S0_T0),
	    .pad_S1_T14_in(data_in_S0_T0),
	    .pad_S1_T15_in(data_in_S0_T0),
	    
	    .pad_S2_T0_in(data_in_S2_T0),
	    .pad_S2_T1_in(data_in_S2_T1),
	    .pad_S2_T2_in(data_in_S2_T2),
	    .pad_S2_T3_in(data_in_S2_T3),
	    .pad_S2_T4_in(data_in_S2_T4),
	    .pad_S2_T5_in(data_in_S2_T5),
	    .pad_S2_T6_in(data_in_S2_T6),
	    .pad_S2_T7_in(data_in_S2_T7),
	    .pad_S2_T8_in(data_in_S2_T8),
	    .pad_S2_T9_in(data_in_S2_T9),
	    .pad_S2_T10_in(data_in_S2_T10),
	    .pad_S2_T11_in(data_in_S2_T11),
	    .pad_S2_T12_in(data_in_S2_T12),
	    .pad_S2_T13_in(data_in_S2_T13),
	    .pad_S2_T14_in(data_in_S2_T14),
	    .pad_S2_T15_in(data_in_S2_T15),

	    .pad_S3_T0_in(data_in_S0_T0),
	    .pad_S3_T1_in(data_in_S0_T1),
	    .pad_S3_T2_in(data_in_S0_T2),
	    .pad_S3_T3_in(data_in_S0_T3),
	    .pad_S3_T4_in(data_in_S0_T4),
	    .pad_S3_T5_in(data_in_S0_T5),
	    .pad_S3_T6_in(data_in_S0_T6),
	    .pad_S3_T7_in(data_in_S0_T7),
	    .pad_S3_T8_in(data_in_S0_T8),
	    .pad_S3_T9_in(data_in_S0_T9),
	    .pad_S3_T10_in(data_in_S0_T10),
	    .pad_S3_T11_in(data_in_S0_T11),
	    .pad_S3_T12_in(data_in_S0_T12),
	    .pad_S3_T13_in(data_in_S0_T13),
	    .pad_S3_T14_in(data_in_S0_T14),
	    .pad_S3_T15_in(data_in_S0_T15),
            
	    .pad_S0_T0_out(data_out_S0_T0),
	    .pad_S0_T1_out(data_out_S0_T1),
	    .pad_S0_T2_out(data_out_S0_T2),
	    .pad_S0_T3_out(data_out_S0_T3),	    	    
	    .pad_S0_T4_out(data_out_S0_T4),
	    .pad_S0_T5_out(data_out_S0_T5),
	    .pad_S0_T6_out(data_out_S0_T6),
	    .pad_S0_T7_out(data_out_S0_T7),
	    .pad_S0_T8_out(data_out_S0_T8),
	    .pad_S0_T9_out(data_out_S0_T9),
	    .pad_S0_T10_out(data_out_S0_T10),
	    .pad_S0_T11_out(data_out_S0_T11),
	    .pad_S0_T12_out(data_out_S0_T12),
	    .pad_S0_T13_out(data_out_S0_T13),
	    .pad_S0_T14_out(data_out_S0_T14),
	    .pad_S0_T15_out(data_out_S0_T15),	    
	    
	    .tdi(tdi),
	    .tms(tms),
	    .tck(tck),
	    .tdo(tdo),
	    .trst_n(trst_n));
   
endmodule
