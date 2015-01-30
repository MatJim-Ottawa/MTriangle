function [out] = MTriangulate(Vertices,Vertices_Boundary_Markers,Vertices_Attributes,Segments, varargin)
% MTriangulate Triangle.c wrapper function
%   Triangulate a planar straight line graph using the Triangle library (http://www.cs.cmu.edu/~quake/triangle.html).
%   The triangulation is a constrained conforming Delaunay triangulation in which additional vertices, called Steiner points,
%   can be inserted into segments to improved the quality of the triangulation. To prevent the insertion of Steiner points on boundary segments,
%   specify Y=1. If the maximum triangle area a is specified, the area of each triangle is not allowed to exceed this value.
%   If the the minimum angle q is specified, no triangle angle is allowed to be below this value.
%
%
%  Usage:
%   - Run 'compile' in MTriangle directory on your system to get a system compatible binary of the C
%    library
%   - MTriangulate can now be used
%
%  Arguments:
%    The first 4 arguments are required.  The remaining arguments each
%    have default values, can be omitted. 
%
%  Vertices                   ... A 2-column matrix of x-y co-ordinates of vertices. 
%                                 There is one row per vertex.
%  Vertices_Boundary_Markers  ... Vector of boundary markers of vertices. For each vertex this is 1 if the point
%                                 should be on a boundary of any mesh generated and 0 otherwise. 
%                                 There should be as many elements as there are vertices in Vertices.
%  Vertices_Attributes        ... Matrix of attributes which are typically floating-point values of physical quantities
%                                 (such as mass or conductivity) associated with the nodes of a finite element mesh. 
%                                 When triangulating using triangulate these are copied unchanged to existing points in the output mesh
%                                 and each new Steiner point added to the mesh will have quantities assigned to it by linear interpolation.
%  Segments                   ... A 2-column matrix of segments in which each row is a segment. Segments are edges whose endpoints are vertices,
%                                 and whose presence in any mesh is enforced. Each segment refers to 
%                                 the indices in Vertices of the endpoints of the segment. By default the segments are not specified (NA),
%                                 in which case the convex hull of the vertices are taken to be the segments. Any vertices outside the 
%                                 region enclosed by the segments are eaten away by the triangulation algorithm. If the segments do not
%                                 enclose a region the whole triangulation may be eaten away.
%  Segments_Boundary_Markers  ... Vector of boundary markers of segments. For each segment this is 1 if the segment should be on a boundary
%                                 of any mesh and 0 otherwise. There should be as many elements as there are segments in Segments.    
%  Holes                      ... 2-column matrix of holes, with one hole per row.Holes are specified by identifying a point inside each hole.
%                                 After the triangulation is formed, Triangle creates holes by eating triangles, spreading out from each hole point
%                                 until its progress is blocked by PSLG segments; you must be careful to enclose each hole in segments, 
%                                 or your whole triangulation might be eaten away. If the two triangles abutting a segment are eaten,
%                                 the segment itself is also eaten. Do not place a hole directly on a segment; if you do,
%                                 Triangle will choose one side of the segment arbitrarily.
%  Verbosity                  ... Integer representing verbosity level. 1 = Medium. 2 = High.
%  Max_Area                   ... Maximum triangle area. If specified, triangles cannot be larger than this area.
%  Min_Angle                  ... Minimum triangle angle in degrees.
%  Steiner_Flag               ... If TRUE prohibits the insertion of Steiner points on the mesh boundary.
%  Jettison_Flag              ... If TRUE jettisons vertices that are not part of the final triangulation from the output.                                
%  Delaunay_Flag              ... If TRUE produce a conforming Delaunay triangulation. This ensures that all the triangles in the mesh are truly Delaunay,
%                                 and not merely constrained Delaunay. This option invokes Ruppert's original algorithm, which splits
%                                 every subsegment whose diametral circle is encroached. It usually increases the number of vertices and triangles.
%  Max_Steiner_Points         ... Specifies the maximum number of added Steiner points.
%
%  Output:
%    Struct object contains the follow attributes:
%
%  Vertices                   ... A 2-column matrix of x-y co-ordinates of vertices. 
%                                 There is one row per vertex.
%  Vertices_Boundary_Markers  ... Vector of boundary markers of vertices. For each vertex this is 1 if the point
%                                 should be on a boundary of any mesh and 0 otherwise. 
%                                 There should be as many elements as there are vertices in Vertices.
%  Vertices_Attributes        ... Matrix of attributes which are typically floating-point values of physical quantities
%                                 (such as mass or conductivity) associated with the nodes of a finite element mesh. 
%  Segments                   ... A 2-column matrix of segments in which each row is a segment. 
%  Segments_Boundary_Markers  ... Vector of boundary markers of segments. For each segment this is 1 if the segment should be on a boundary
%                                 of any mesh and 0 otherwise. There should be as many elements as there are segments in Segments.
%  Triangulation              ... Triangulation specified as 3 column matrix in which each row contains indices in Vertices.
%  Edges                      ... Set of edges in the triangulation.
%  Edge_Boundary_Markers      ... Boundary markers of edges. For each edge this is 1 if the point is on a boundary
%                                 of the triangulation and 0 otherwise.
%  Voronoi_Points             ... The points of the Voronoi tessalation as a 2-column matrix
%  Voronoi_Edges              ... Set of edges of the Voronoi tessalation. An index of -1 indicates an infinite ray.
%  Voronoi_Directions         ... Directions of infinite rays of Voroni tessalation as a 2-column matrix.
%  Voronoi_Attributes         ... Matrix of attributes associated with the polygons of the Voronoi tessalation.


%  Last modified: 29 January 2015


%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% Create inputParser object. This object will handle parsing and validation
% of function inputs

p = inputParser;

%%%%%%%%%%%%%%%%%%%%%
% Setup validation functions for inputs. These functions return true if
% input is valid. False otherwise.

% Required Inputs


% Optional Inputs

Validate_Max_Area = @(x) assert(isnumeric(x) && isscalar(x) ...
   && x > 0 , 'Value must be positive, scalar and numeric.');

Validate_Min_Angle = @(x) assert(isnumeric(x) && isscalar(x) ...
   && x > 0 && x < 45, 'Value must be positive, scalar and numeric less than 45.');

Validate_Steiner_Flag = @(x)  assert(isnumeric(x) && isscalar(x) && ( x == 0 || x==1));

Validate_Jettison_Flag = @(x)  assert(isnumeric(x) && isscalar(x) && ( x == 0 || x==1));

Validate_Delaunay_Flag = @(x)  assert(isnumeric(x) && isscalar(x) && ( x == 0 || x==1));

Validate_Verbosity = @(x) assert(isscalar(x) && isinteger(x) ...
   && x > 0 , 'Value must be positive, scalar and integer.');

Validate_Max_Steiner_Points = @(x) assert(isscalar(x) && isinteger(x) ...
   && x > 0 , 'Value must be positive, scalar and integer.');

%%%%%%%%%%%%%%%%%%%%%
% Register expected inputs with 
addRequired(p,'Vertices');
addRequired(p,'Vertices_Boundary_Markers');
addRequired(p,'Vertices_Attributes');
addRequired(p,'Segments');

% Optional Parameters are specified by including 'NAME', VALUE pair
% arguments in the function call after the required inputs..
% eg. MTriangulate(..., 'Max_Area',0.01);

addParameter(p,'Max_Area',[],Validate_Max_Area);
addParameter(p,'Min_Angle',[],Validate_Min_Angle);
addParameter(p,'Steiner_Flag',[],Validate_Steiner_Flag);
addParameter(p,'Jettison_Flag',0,Validate_Jettison_Flag);
addParameter(p,'Delaunay_Flag',0,Validate_Delaunay_Flag);
addParameter(p,'Verbosity',0,Validate_Verbosity);
addParameter(p,'Max_Steiner_Points',[],Validate_Max_Steiner_Points);

% Call function to parse functions arguments with inputParser p
parse(p,Vertices,Vertices_Boundary_Markers,Vertices_Attributes,Segments, varargin{:});

% Object p will now contain all the inputs, which can be accessed by dot
% notation
% eg   p.Vertices


%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% Build string concatinating switches used in Triangle library depending on
% options selected

out = [];
result = 'pevn';



    if isempty(p.Results.Segments)         
        result = strcat(result,'c');  
    end
    
    if ~isempty(p.Results.Max_Area)
         result = strcat(result,'a',num2str(p.Results.Max_Area));
    end
    
    if ~isempty(p.Results.Min_Angle)
        result = strcat(result,'q',num2str(p.Results.Min_Angle));
    end
    
    if ~isempty(p.Results.Steiner_Flag)
        if p.Results.Steiner_Flag
            result = strcat(result,'Y');
        end
    end
    
    if ~isempty(p.Results.Max_Steiner_Points)
        result = strcat(result,'S',num2str(p.Results.Max_Steiner_Points));
    end
    
   
    if ~isempty(p.Results.Jettison_Flag)
        if p.Results.Jettison_Flag
            result = strcat(result,'j');
        end
    end    

    if ~isempty(p.Results.Delaunay_Flag)
        if p.Results.Delaunay_Flag
            result = strcat(result,'D');
        end
    end        

    if ~isempty(p.Results.Verbosity)
        if p.Results.Verbosity == 1
            result = strcat(result,'V');
        else if p.Results.Verbosity == 2
            result = strcat(result,'VV');
        end
    end    

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% Call the MATLAB compiled C function with necessary inputs
    
[out.Vertices,out.Vertices_Boundary_Markers,out.Vertices_Attributes,out.Triangulation,out.Segments,out.Segments_Boundary_Markers,out.Edges,out.Edge_Boundary_Markers,out.Voronoi_Points,out.Voronoi_Edges,out.Voronoi_Directions,out.Voronoi_Attributes] = ...
    minterface(p.Results.Vertices,p.Results.Vertices_Boundary_Markers,p.Results.Vertices_Attributes,p.Results.Segments,result);

end

