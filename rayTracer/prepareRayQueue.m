%
% PREPARERAYQUEUE
%
% rays = prepareRayQueue(...
%   bSys, Tprop, theta, omega, numRays, timeStep, sampleEach, ...
%   fnameBinarySystem, fnameRayInitCondQueue, fnameRayPathQueue, ...
%   rHat0, kHat0, maxSamples)

function rays = prepareRayQueue(...
  bSys, Tprop, theta, omega, numRays, timeStep, sampleEach, ...
  fnameBinarySystem, fnameRayInitCondQueue, fnameRayPathQueue, ...
    rHat0, kHat0, maxSamples)

  constants;
	rHat0Given = ~isempty(rHat0);
  kHat0Given = ~isempty(kHat0);
  if isempty(maxSamples)
    maxTime = 1 / (2*houdekEta(omega / (2*pi)));
    maxSamples = round(maxTime / (timeStep * sampleEach));
  end

  distUpper = 1.4; % upper bound for binary radial distortion factor
  distLower = 0.8;  
	R = bSys.sst.R(end);
	drApprox = R * 1e-4;  
  
	for i = 1:numRays
    
    if ~rHat0Given; rHat0 = isotropicUnitVector(1); end
		r1 = bSys.r1 +  rHat0 * distLower * R;
    r2 = bSys.r1 +  rHat0 * distUpper * R;   
    
		[rx,ry,rz] = coordsRay(r1, r2, drApprox);
		T = sampleStructure(bSys, bSys.sst.T, rx, ry, rz);
    dr = norm(r2 - r1) / (numel(T) - 1);
    T = T(1:find(isnan(T), 1, 'first') - 1);
    
    r0 = bSys.r1 + interp1(T,distLower*R + [0:numel(T)-1]*dr,Tprop)*rHat0;
		[g, gHat] = sampleG(bSys, r0(1), r0(2), r0(3));

    if ~kHat0Given;
      tangHat = isotropicTangent(gHat);
      kHat0 = gHat * cos(theta) + tangHat * sin(theta);
    end
    
		rays{i} = initRay(bSys, r0, kHat0, 1, omega, ...
      timeStep, sampleEach, maxSamples);
  end
  
  warning('OFF', 'MATLAB:DELETE:FileNotFound');  
	delete(fnameBinarySystem);
	delete(fnameRayInitCondQueue);
	delete(fnameRayPathQueue);
  warning('ON', 'MATLAB:DELETE:FileNotFound');
	
	enqueueBinarySystem(bSys, fnameBinarySystem);
  
	for i = 1:numRays
		enqueueRay(rays{i}, fnameRayInitCondQueue);
	end
