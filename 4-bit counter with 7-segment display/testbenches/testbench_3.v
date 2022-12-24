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

`define info(message) \
    $write("\nTIME "); $display($realtime); \
    $display("INFO %s", message);
// End of `info macro.

// It looks like this testbench contains a "$finish" system task.
// Be aware that some simulation programs (e.g. Modelsim) will immediately close once the "$finish" system task is executed.
// If you find that your simulation program is closing unexpectedly, try removing any "$finish" system tasks in this file.

`timescale 1 ns / 1 ns
module testbench;
   reg enable, upDown, clk, rst;
   wire [3:0] count;
   wire [6:0] seg7;

    Top dut(clk, rst, enable, upDown, count, seg7);
    initial clk = 1;  //init clk = 1 for positive-edge triggered
    always begin  // (undivided) clk wave, period of 2
      #1  clk = ~clk;
    end
    
   always begin  // display I/O every 20 ns
      #20 $display("Time=%3d enable=%b upDown=%b count=%x seg7=%b", $realtime, enable,upDown,count,seg7);
   end

    initial begin
      // Reset
      rst = 1; enable = 1; upDown = 0;
      #10  rst = 0;
      `info("Starting down count... ");
   
      `assert(seg7, 7'b0000001,"seg7=0","enable=1 upDown=0", "count fails at 0 after Reset","%b")

       #20;
      `assert(seg7, 7'b0111000,"seg7=F downcounting after wraparound","enable=1 upDown=0", "count fails at F after downcount wrap","%b")
      upDown = 1;
      `info("Switching to counting up... ");
       #20;
      `assert(seg7, 7'b0000001,"seg7=0","enable=1 upDown=1", "count fails at 0, after upcount wraparound","%b")
      #20;
      `assert(seg7, 7'b1001111,"seg7=1","enable=1 upDown=1", "count fails at 1","%b")
      #20;
      `assert(seg7, 7'b0010010,"seg7=2","enable=1 upDown=1", "count fails at 2","%b")
      #20;
      `assert(seg7, 7'b0000110,"seg7=3","enable=1 upDown=1", "count fails at 3","%b")
      #20;
      `assert(seg7, 7'b1001100,"seg7=4","enable=1 upDown=1", "count fails at 4","%b")
      #20;
      `assert(seg7, 7'b0100100,"seg7=5","enable=1 upDown=1", "count fails at 5","%b")
      #20;
      `assert(seg7, 7'b0100000,"seg7=6","enable=1 upDown=1", "count fails at 6","%b")
      #20;
      `assert(seg7, 7'b0001111,"seg7=7","enable=1 upDown=1", "count fails at 7","%b")
      #20;
      `assert(seg7, 7'b0000000,"seg7=8","enable=1 upDown=1", "count fails at 8","%b")
      #20;
      `assert(seg7, 7'b0000100,"seg7=9","enable=1 upDown=1", "count fails at 9","%b")
      #20;
      `assert(seg7, 7'b0001000,"seg7=A","enable=1 upDown=1", "count fails at A","%b")
      #20;
      `assert(seg7, 7'b1100000,"seg7=b","enable=1 upDown=1", "count fails at b","%b")
      #20;
      `assert(seg7, 7'b0110001,"seg7=C","enable=1 upDown=1", "count fails at C","%b")
      #20;
      `assert(seg7, 7'b1000010,"seg7=d","enable=1 upDown=1", "count fails at d","%b")
      #20;
      `assert(seg7, 7'b0110000,"seg7=E","enable=1 upDown=1", "count fails at E","%b")
      #20;
      `assert(seg7, 7'b0111000,"seg7=F","enable=1 upDown=1", "count fails at F","%b")
      
      `info("Reached maximum count, wrapping around to zero..");
      #20;
      `assert(seg7, 7'b0000001,"seg7=0","enable=1 upDown=1", "count fails at 0, after upcount wraparound","%b")
     $finish;

    end
endmodule