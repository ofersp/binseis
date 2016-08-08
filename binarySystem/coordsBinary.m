% 
% COORDSBINARY Convert cartesian to binary coords.
%
% Given the center of mass of the two binary members, r1 and r2, and cartesian 
% coords. X,Y,Z coordsBinary() returns R1, R2 and Rxy which are the distance of 
% given coords. to r1, r2 and to the origin in the x,y plane, respectively. 
% Returned matrices have the same matrix dimensions as X,Y and Z.
%
% [R1, R2, Rxy] = coordsBinary(r1, r2, X, Y, Z)

function [R1, R2, Rxy] = coordsBinary(r1, r2, X, Y, Z)

	gridDim = size(X);
	Pos = [X(:) Y(:) Z(:)];
	PosRelR1 = Pos - ones(size(Pos,1), 1) * r1;
	PosRelR2 = Pos - ones(size(Pos,1), 1) * r2;

	R1 = reshape(sqrt(PosRelR1(:,1).^2 + PosRelR1(:,2).^2 + PosRelR1(:,3).^2), gridDim);
	R2 = reshape(sqrt(PosRelR2(:,1).^2 + PosRelR2(:,2).^2 + PosRelR2(:,3).^2), gridDim);
	Rxy = reshape(sqrt(X(:).^2 + Y(:).^2), gridDim);
