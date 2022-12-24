`define assert(actual, expected, outputName, inputs, description, type) \
    $write("\nTIME: "); $write($realtime); \
    if (actual == expected) \
        $display("   PASSED:   "); \
    else begin \
        $display(" ** FAILED:   "); \
        if (description) $display("TEST: %s", description); \
    end \
    $write("%s = ", outputName, type, actual); \
    $write(", EXPECTED: ", type, expected); \
    if (inputs) $display(" WITH: %s", inputs);
// End of `assert macro.

// It looks like this testbench contains a "$finish" system task.
// Be aware that some simulation programs (e.g. Modelsim) will immediately close once the "$finish" system task is executed.
// If you find that your simulation program is closing unexpectedly, try removing any "$finish" system tasks in this file.

// Testbench for Arbier module
//  Test reset
`timescale 1 ns / 1 ns
module testbench();
                                        
reg clk,rst,ra,rb;  // inputs 
reg [1:0] PA,PB;  
wire ga,gb;         // outputs  

Arbiter uut (clk,rst,ra,rb,PA,PB,ga,gb);

initial clk = 1; // clk = 1 means inputs will transition on clk rising edge
 always begin    // Clk wave, period of 10
      #5  clk = ~clk;
 end

initial
 begin
  rst = 1; ra = 0; rb = 0; PA = 2'b0; PB = 2'b0; //reset 
  #10  // t = 10
   rst = 0;  // expect everything = 0 when reset 
  `assert(ga, 1'b0, "ga","rst=1","reset makes all outputs 0", "%b")
  `assert(gb, 1'b0, "gb","rst=1","reset makes all outputs 0", "%b")
   #10  $finish; 
 end 
endmodule