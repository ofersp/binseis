%
% ISOTROPICUNITVECTOR Generates spherical coords of random (isotropic) unit vectors.
%
% [v,theta,phi] = isotropicUnitVector(N)

function [v,theta,phi] = isotropicUnitVector(N)

	phi = 2*pi*rand(N,1);
	u = rand(N,1) - 0.5;
	theta = acos(-2*u);
	v = [sin(theta).*sin(phi), sin(theta).*cos(phi), cos(theta)];
