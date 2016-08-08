%
% INNERTURNINGPOINT Returns radius of inner turning point of a ray in bSys.
%
% For given binary system bSys, returns the radius of the inner turning
% point, RTO, for given values of theta and omega. theta are the angles 
% in radians, at the surface, between the initial ray directions and the 
% surface normal and omega are the ray angular frequencies. The ray 
% initial propagation surface is defined by T = Tprop
%
% see: JCD - Stellar Oscillations, formula 5.28
%
% RTO = innerTurningPoint(bSys, theta, omega, Tprop)

function RTO = innerTurningPoint(bSys, theta, omega, Tprop)
	
	timeStep = 2;
	sampleEach = 1;
	maxSamples = 3300;

	fnameBinarySystem = 'io/binarySystem.bin';
	fnameRayInitCondQueue = 'io/rayInitCondQueue.bin';
	fnameRayPathQueue = 'io/rayPathQueue.bin';

	delete(fnameBinarySystem);
	delete(fnameRayInitCondQueue);
	delete(fnameRayPathQueue);
	
	enqueueBinarySystem(bSys, fnameBinarySystem);
  
  Rprop = interp1(bSys.sst.T, bSys.sst.R, Tprop);  
	r0 = bSys.r1 + [Rprop,0,0];

	n = 0;
  for j=1:numel(omega)
  for i=1:numel(theta)		
    kHat0 = [-cos(theta(i)), 0, sin(theta(i))];
    n = n + 1;
    rays{n} = initRay(bSys, r0, kHat0, 1, omega(j), ...
      timeStep, sampleEach, maxSamples);
    enqueueRay(rays{n}, fnameRayInitCondQueue);
  end
	end	

	system('trace/trace');
	rayPaths = dequeueRayPaths(fnameRayPathQueue, n, false);

	RTO = zeros(numel(theta), numel(omega));

	n = 0;
	for j=1:numel(omega)
	for i=1:numel(theta)
    n = n + 1;
		r = rayPaths{n}.r - ones(maxSamples,1)*bSys.r1;
		R = sqrt(sum(r.^2, 2));
		RTO(i,j) = min(R);
	end
  end
