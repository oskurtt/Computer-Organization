// 4-bit up/down counter
module Count4(clk,rst,enable,upDown, count);
   input clk, rst, enable, upDown;
   output reg [3:0] count;

   // insert code here 
   always @(posedge clk)
   begin
      if(rst)
         count =0;
      else
         if(enable)
            if(upDown)
               count = count + 1;
            else
               count = count - 1;
   end
endmodule  // Count4