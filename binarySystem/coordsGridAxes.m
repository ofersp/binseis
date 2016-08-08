%
% COORDSGRIDAXES Generate grid axes given corners C1,C2 and step spaceStep.
%
% [x, y, z] = coordsGridAxes(C1, C2, spaceStep);

function [x, y, z] = coordsGridAxes(C1, C2, spaceStep)

	x = C1(1):spaceStep:C2(1);
	y = C1(2):spaceStep:C2(2);
	z = C1(3):spaceStep:C2(3);