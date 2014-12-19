function [out] = MTriangle(P,PB,PA,S)
%MTriangle Summary of this function goes here
%   Detailed explanation goes here
out = [];

[out.P,out.PB,out.PA,out.T,out.S,out.SB,out.E,out.EB,out.VP,out.VE,out.VN,out.VA] = minterface(P,PB,PA,S);
end

