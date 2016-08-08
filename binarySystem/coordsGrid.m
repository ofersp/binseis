%
% COORDSGRID Generate grid given corners C1,C2 and step spaceStep.
%
% [X, Y, Z] = coordsGrid(C1, C2, spaceStep)

function [X, Y, Z] = coordsGrid(C1, C2, spaceStep)

	[x,y,z] = coordsGridAxes(C1, C2, spaceStep); 
	[X,Y,Z] = ndgrid(x, y, z);
