%
% ISOTROPICTANGENT Returns a random (isotropic) tangent to given vector.
%
% bHat = isotropicTangent(a)

function bHat = isotropicTangent(a)
	
	epsilon = 1e-4;
	zHat = a / norm(a);
	
	if 1-zHat(1) < epsilon
		u = [0,1,0];
	else
		u = [1,0,0];
	end
	
	xHat = cross(zHat, u); xHat = xHat / norm(xHat);
	yHat = cross(zHat, xHat); yHat = yHat / norm(yHat);
	
	theta = 2*pi*rand(1);
	bHat = xHat*cos(theta) + yHat*sin(theta);
