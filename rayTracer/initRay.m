%
% INITRAY Initialize ray init cond struct given its params.
%
% ray = initRay(bSys,r0,kHat0,branch,omega,timeStep,sampleEach,maxSamples)

function ray = initRay(bSys, r0, kHat0, branch, omega, timeStep, ...
  sampleEach, maxSamples)

  kHat0 = kHat0 / norm(kHat0);
  
	% initial ray conditions

	ray.r0 = r0;
	ray.k0 = kHat0 * kOfOmega(bSys, r0, kHat0, branch, omega);
	ray.branch = branch;
	ray.omega = omega;

	ray.timeStep = timeStep;	
	ray.sampleEach = sampleEach;
	ray.maxSamples = maxSamples;
  ray.maxSteps = maxSamples * ray.sampleEach;


function k = kOfOmega(bSys, r, kHat, branch, omega)

	% sample the following fields at r

	[g, gHat] = sampleG(bSys, r(1), r(2), r(3));
	P = sampleStructure(bSys, bSys.sst.P, r(1), r(2), r(3));
	rho = sampleStructure(bSys, bSys.sst.rho, r(1), r(2), r(3));
	Gamma1 = sampleStructure(bSys, bSys.sst.Gamma1, r(1), r(2), r(3));

	aSqr = (P/rho)*Gamma1;
	omegaASqr = (Gamma1^2 * g^2) / (4 * aSqr);
	omegaGSqrOverOmegaASqr = (Gamma1 - 1) / (4*Gamma1^2);
	omegaGSqr = omegaASqr * omegaGSqrOverOmegaASqr;

  kSqr = kSqrOfOmegaGough(omega, aSqr, omegaASqr);  
	k = sqrt(kSqr);

  % check that the requested frequency is in the expected branch
  
  if kSqr < 0 
    warning('kOfOmega:nonPropagating', 'Required frequency is non-propagating');
  elseif omega^2 > omegaASqr && branch == -1
    warning('kOfOmega:notGravitational', 'Required frequency is not on the gravitational branch');
  elseif omega^2 < omegaASqr && branch == 1
    warning('kOfOmega:notAcoustic', 'Required frequency is not on the acoustic branch');
	elseif isnan(kSqr)
		warning('kOfOmega:outOfStar', 'sampleStructure at r0 produced a NaN');		
  end

%
%

function kSqr = kSqrOfOmegaGough(omega, aSqr, omegaASqr)

  kSqr = (omega^2 - omegaASqr) / aSqr;
  
%
%
 
function kSqr = kSqrOfOmegaIsothermal(kHat, omega, g, gHat, aSqr, ...
  omegaASqr, omegaGSqr)
  
  k1 = sqrt(1 - sum(kHat .* gHat)^2);
	kSqr = (omegaASqr - omega^2) / ((k1^2 * omegaGSqr / omega^2 - 1) * aSqr);
  
