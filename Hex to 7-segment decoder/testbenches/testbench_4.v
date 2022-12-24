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
    reg [3:0] bin;  // inputs to dut (Device Under Test)
    wire [6:0] seg7;              // dut output
    Seg7decode dut(bin, seg7);  // instatntiate dut to test
/////////////////////////////////////////////////////////////
    initial bin = 12;   
    always begin   
        #10  bin = bin + 1;
    end

    initial begin  // Create sequence of inputs 
     #3   `assert(seg7, 7'b0110001,"seg7 should be 0110001 and","bin = C","seg7 should be 0110000","%b")
     #10; `assert(seg7, 7'b1000010,"seg7 should be 1000010 and","bin = d","seg7 should be 1000010","%b")
     #10; `assert(seg7, 7'b0110000,"seg7 should be 0110000 and","bin = E","seg7 should be 0110000","%b")
     #10; `assert(seg7, 7'b0111000,"seg7 should be 0111000 and","bin = F","seg7 should be 0111000","%b")
     $finish;
    end
endmodule