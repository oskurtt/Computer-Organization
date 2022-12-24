//  Two-way arbiter FSM/HLSM.  
//   Similar to the example in the HLSM chapter, but eliminates extra state ("Arbitrate") and uses meaningful signal names  
module Arbiter(
   input clk,rst,ra,rb, // ra = request from requestor a, rb = request from requestor b, which are level (not pulse) signals
   input [1:0] PA, PB,  // priority values for requestor a and requestor b, unsigned, higher number is higher priority
   output reg ga,gb     // ga = grant to a, gb = grant to b
);
   // insert code here
   reg [1:0] State;
   localparam [1:0] WaitReq = 0, GrantA = 1, GrantB = 2;
   always @(posedge clk)
   begin
   if (rst)
      begin
      ga = 0;
      gb = 0;
      State = WaitReq;
      end
   else
      begin
      //if (State == WaitReq)
         //begin
         if (ra && !rb)
            begin
            State = GrantA;
            ga = 1;
            gb = 0;
            end
         else if (!ra && rb)
            begin
            State = GrantB;
            ga = 0;
            gb = 1;
            end
         else if (!ra && !rb)
            begin
            State = WaitReq;
            ga = 0;
            gb = 0;
            end
         else if (ra && rb)
            begin
            State = GrantB;
            if (PB > PA)
               begin
               gb = 1;
               ga = 0;
               end
            else  
               begin
               State = GrantA;
               ga = 1;
               gb = 0;
               end
            end
         //end
      end
   end


endmodule