module Top(clk, rst, enable, upDown, count, seg7);
// count[3:0] output included for convenience in debug and testbenches
    input clk, rst, enable, upDown;
    output [3:0] count;
    output [6:0] seg7;

   wire clkCounter;  // divided clock for Counter module

    // Add code here to instantiate and connect the three modules together structurally
    ClkDiv ClkDiv1(clk,clkCounter);
    Count4 Counter(clkCounter,rst,enable,upDown,count);
    Seg7decode SevenSeg(count, seg7);
endmodule