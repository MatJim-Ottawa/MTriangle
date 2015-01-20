function [] = MTriangle_plot( results )
%MTRIANGLE_PLOT Plot the results of Triangulate

f = figure();

A = sparse(results.E(:,1),results.E(:,2),1);

hold on;
gplot(A,results.P,'-b');
plot(results.P(:,1),results.P(:,2),'ro');





hold off;


end

