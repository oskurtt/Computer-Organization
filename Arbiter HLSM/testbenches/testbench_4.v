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
//  Simultaneous requests, PA greater
`timescale 1 ns / 1 ns

module testbench();
                                        
reg clk,rst,ra,rb; // inputs 
reg [1:0] PA,PB;  
wire ga,gb;        // outputs  

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
  #10  // t = 20
   ra = 1; rb = 0; // ra alone requests
  #10  // t = 30   // Expecting ga =1 (grant to A)
  `assert(ga, 1'b1, "ga","ra = 1, rb = 0","ga output = 1 when ra alone requests", "%b")
  `assert(gb, 1'b0, "gb","ra = 1, rb = 0","gb output = 0 when ra alone requests", "%b")
 
   #20  // t = 50
   ra = 1; rb = 1; // overlap one clock of ra = 1 rb = 1
   #10  // t = 60
     // Expecting ga =1 (continue to grant to A when rb =1 if A already owns the resource)
  `assert(ga, 1'b1, "ga","ra = 1, rb = 1","ga output = 1 when ra owns the resource", "%b")
  `assert(gb, 1'b0, "gb","ra = 1, rb = 1","gb output = 0 when ra owns the resource", "%b")

    ra = 0; rb = 1; // rb = 1 alone 
     
    #20  // t = 80   // Expecting gb =1 
  `assert(ga, 1'b0, "ga","ra = 0, rb = 1","ga output = 0 when rb alone requests", "%b")
  `assert(gb, 1'b1, "gb","ra = 0, rb = 1","gb output = 1 when rb alone requests", "%b")
    ra = 1;
 
    #10 // t = 90 
    ra = 1; rb = 0;
    
    #30 // t = 120  // Expecting ga = 1 still due to earlier ra = 1 
   `assert(ga, 1'b1, "ga","ra = 1, rb = 0","ga output = 1 when ra alone requests", "%b")
   `assert(gb, 1'b0, "gb","ra = 1, rb = 0","gb output = 0 when ra alone requests", "%b")
    ra = 0; rb = 0;
    
    #10 // t = 130 
    PB = 3; // raise B's priority higher than A's
    #10 // t = 140  
    ra = 1; rb = 1; // simultaneous requests, PB greater
 
    #30  // t = 170   // Expecting gb =1 
    `assert(ga, 1'b0, "ga","ra = 1, rb = 1, PB > PA","ga output = 0 for simultaneous requests when PB>PA", "%b")
    `assert(gb, 1'b1, "gb","ra = 1, rb = 1, PB > PA","gb output = 1 for simultaneous requests when PB>PA", "%b")
    ra = 0; rb = 0;

    #10 // t = 180 
    PA = 3; PB = 0; // raise A's priority higher than B's
 
    #10 // t = 190 
    ra = 1; rb = 1;  // simultaneous requests, PA greater

    #30  // t = 220   // Expecting ga = 1 
    `assert(ga, 1'b1, "ga","ra = 1, rb = 1, PA > PB","ga output = 1 for simultaneous requests when PA>PB", "%b")
    `assert(gb, 1'b0, "gb","ra = 1, rb = 1, PA > PB","gb output = 0 for simultaneous requests when PA>PB", "%b")
    ra = 0; rb = 0;

    #30  // t = 250  
    `assert(ga, 1'b0, "ga","ra = 0, rb = 0","ga output = 0 when no requests", "%b")
    `assert(gb, 1'b0, "gb","ra = 0, rb = 0","gb output = 0 when no requests", "%b")
     PA = 0; PB = 0; // reset priorities equal 
 
     #20 // t = 270 
     ra = 1; rb = 1;  // simultaneous requests, PA = PB, so ga should win

     #10  // t = 280   // Expecting ga = 1 
    `assert(ga, 1'b1, "ga","ra = 1, rb = 1, PA = PB","ga output = 1 for simultaneous requests when PA=PB", "%b")
    `assert(gb, 1'b0, "gb","ra = 1, rb = 1, PA = PB","gb output = 0 for simultaneous requests when PA=PB", "%b")

     #20 // t = 300 
     ra = 0; rb = 0;   
     #10  // t = 310  
    `assert(ga, 1'b0, "ga","ra = 0, rb = 0","ga output = 0 when no requests", "%b")
    `assert(gb, 1'b0, "gb","ra = 0, rb = 0","gb output = 0 when no requests", "%b")


  #10  $finish;
 end 
endmodule