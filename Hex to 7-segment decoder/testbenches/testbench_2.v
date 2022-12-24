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
    initial bin = 4;   
    always begin   
        #10  bin = bin + 1;
    end

    initial begin  // Create sequence of inputs 
     #3   `assert(seg7, 7'b1001100,"seg7 should be 1001100 and","bin = 4","seg7 should be 1001100","%b")
     #10; `assert(seg7, 7'b0100100,"seg7 should be 0100100 and","bin = 5","seg7 should be 0100100","%b")
     #10; `assert(seg7, 7'b0100000,"seg7 should be 0100000 and","bin = 6","seg7 should be 0100000","%b")
     #10; `assert(seg7, 7'b0001111,"seg7 should be 0001111 and","bin = 7","seg7 should be 0001111","%b")
     $finish;
    end
endmodule