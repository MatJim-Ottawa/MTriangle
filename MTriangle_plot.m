function [] = MTriangle_plot( results )
%MTRIANGLE_PLOT Plot the results of Triangulate

f = figure();

A = sparse(results.Edges(:,1),results.Edges(:,2),1);

hold on;
gplot(A,results.Vertices,'-b');
plot(results.Vertices(:,1),results.Vertices(:,2),'ro');





hold off;


end

