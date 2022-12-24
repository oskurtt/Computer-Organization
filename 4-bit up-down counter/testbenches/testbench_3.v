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

`timescale 1 ns / 1 ns

module testbench;
   reg clk, rst, enable, upDown;  // inputs to dut (Device Under Test)
   wire [3:0] count;              // dut output
   reg [25*8:0] input_string;  //string for printout of inputs in the assert macro

   Count4 dut(clk,rst,enable,upDown,count);  // instatntiate dut to test
 /////////////////////////////////////////////////////////////
 // Alternative model of counter, to compare dut outputs
   reg [3:0] C = 0; 
   always @(posedge clk) begin 
      if (rst) C = 0;
      else 
         if (upDown) C = C + enable;
         else C = C - enable;
   end // always
/////////////////////////////////////////////////////////////
   always @(negedge clk) begin  //display all values, check count at each clock
     $sformat(input_string, "rst=%b enable=%b upDown=%b",rst,enable,upDown);
     `assert(count,C,"count",input_string,"Error","%d")
   end // the always     
/////////////////////////////////////////////////////////////
   initial clk = 1;  //init clk = 1 for positive-edge triggered
   always begin  // clk wave, period of 10
      #5  clk = ~clk;
   end

   initial begin  // Create sequence of inputs to check upDown, rst, enable functions
      // Reset
      rst = 1;
      enable = 1; upDown = 1;        // enable counting up
  
      #10; rst = 0;
      `assert(count, 4'b0000,"count","rst=1 enable=1 upDown=1", "count=0","%d")
      #120; // 12 clocks have elapsed.  
//      `assert(count, 4'b1100,"count","rst=0 enable=1 upDown=1", "count up to 12","%d")
      #30; 
//      `assert(count, 4'b1111,"count","rst=0 enable=1 upDown=1", "count up to 15","%d")
      #5;
      `assert(count, 4'b0000,"count","rst=0 enable=1 upDown=1, does count wrap from 15 to 0?", "count wraps from 15 to 0 upcounting?","%d")
      #10;
//      `assert(count, 4'b0001,"count","rst=0 enable=1 upDown=1", "Upcounting 0 to 1","%d")
        
      //  Try upDown=0
      upDown = 0; 
      #30; 
      `assert(count, 4'b1110,"count","rst=0 enable=1 upDown=0", "count down to 14","%d")
      #100; // run 10 more clocks  
//      `assert(count, 4'b0100,"count","rst=0 enable=1 upDown=0", "count down to 4","%d")
      #50
      `assert(count, 4'b1111,"count","rst=0 enable=1 upDown=0, does countdown wrap from 0 to 15?", "countdown wraps from 0 to 15?","%d")
        
      $finish;
   end
endmodule