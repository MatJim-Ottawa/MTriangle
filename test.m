
% DATA

P = [0,0;0,1 ;0.5,0.5;1,1;1,0];

S = [1,2;2,3;3,4;4,5;5,1];

PA = [];

PB = [0;0;0;0;0];



o = MTriangle(P,PB,PA,S, 'a' , 0.01);

MTriangle_plot(o);