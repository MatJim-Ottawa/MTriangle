

mex -c -g triangle.c
if isunix
    mex -g minterface.cpp triangle.o
else
    mex -g minterface.cpp triangle.obj
end
    
