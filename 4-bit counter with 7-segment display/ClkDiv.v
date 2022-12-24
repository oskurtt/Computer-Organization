module ClkDiv(clk, clkOut); 
   input clk; 
   output reg clkOut;
  
   parameter divVal = 4; // 4 for divide by 10 (dividing by 2*(DivVal+1)
   reg[3:0] divCnt;
   initial begin divCnt = 0; clkOut = 0; end  // initialize for simulator
   	
   always @(posedge clk) begin
      if (divCnt == divVal) begin
            clkOut = ~clkOut;
            divCnt = 0;
      end
      else begin
            divCnt = divCnt + 1;
      end
   end
endmodule