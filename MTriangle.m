function [out] = MTriangle(P,PB,PA,S, varargin)
%MTriangle Summary of this function goes here
%   Detailed explanation goes here


% Check options
% This is important since they are being passed to C code.


p = inputParser;

Validate_a = @(x) assert(isnumeric(x) && isscalar(x) ...
   && x > 0 , 'Value must be positive, scalar and numeric.');

Validate_q = @(x) assert(isnumeric(x) && isscalar(x) ...
   && x > 0 && x < 60, 'Value must be positive, scalar and numeric less than 60.');

Validate_Y = @(x)  assert(isnumeric(x) && isscalar(x) && ( x == 0 || x==1));

Validate_j = @(x)  assert(isnumeric(x) && isscalar(x) && ( x == 0 || x==1));

Validate_D = @(x)  assert(isnumeric(x) && isscalar(x) && ( x == 0 || x==1));

Validate_V = @(x) assert(isscalar(x) && isinteger(x) ...
   && x > 0 , 'Value must be positive, scalar and integer.');

Validate_Z = @(x)  assert(isnumeric(x) && isscalar(x) && ( x == 0 || x==1));

Validate_SS = @(x) assert(isscalar(x) && isinteger(x) ...
   && x > 0 , 'Value must be positive, scalar and integer.');

addRequired(p,'P');
addRequired(p,'PB');
addRequired(p,'PA');
addRequired(p,'S');
addParameter(p,'a',[],Validate_a);
addParameter(p,'q',[],Validate_q);
addParameter(p,'Y',[],Validate_Y);
addParameter(p,'j',0,Validate_j);
addParameter(p,'D',0,Validate_D);
addParameter(p,'V',0,Validate_V);
addParameter(p,'Z',1,Validate_Z);
addParameter(p,'SS',[],Validate_SS);

parse(p,P,PB,PA,S, varargin{:});

out = [];
result = 'pevn';



    if isempty(p.Results.S)         
        result = strcat(result,'c');  
    end
    
    if ~isempty(p.Results.a)
         result = strcat(result,'a',num2str(p.Results.a));
    end
    
    if ~isempty(p.Results.q)
        result = strcat(result,'q',num2str(p.Results.q));
    end
    
    if ~isempty(p.Results.Y)
        if p.Results.Y
            result = strcat(result,'Y');
        end
    end
    
    if ~isempty(p.Results.SS)
        result = strcat(result,'S',num2str(p.Results.S));
    end
    
    if ~isempty(p.Results.SS)
        result = strcat(result,'S',num2str(p.Results.S));
    end
    
    if ~isempty(p.Results.j)
        if p.Results.j
            result = strcat(result,'j');
        end
    end    

    if ~isempty(p.Results.D)
        if p.Results.D
            result = strcat(result,'D');
        end
    end        

    if ~isempty(p.Results.V)
        if p.Results.V ==1
            result = strcat(result,'V');
        else if p.Results.V ==2
            result = strcat(result,'VV');
        end
    end    

%     if ~isempty(p.Results.Z)
%         if p.Results.Z
%             result = strcat(result,'Q');
%         end
%     end        

    
    
[out.P,out.PB,out.PA,out.T,out.S,out.SB,out.E,out.EB,out.VP,out.VE,out.VN,out.VA] = minterface(p.Results.P,p.Results.PB,p.Results.PA,p.Results.S,result);
end

