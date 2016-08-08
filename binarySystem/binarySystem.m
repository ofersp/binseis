%
% BINARYSYSTEM Load the stellar structure of a binary and init bSys struct.
%
% Load the stellar structure of a binary member and initialize binary system 
% parameters and Kippenhahn-Thomas tables. starSetName is the path to the 
% structure set and starIndex is the star's index withing the set. e.g. given -
% starSetName = '../stellarStructure/io/SC/SC' with starIndex = 7, will load the 
% binary system defined by the stellarStructure file '../stellarStructure/io/SC/SC07.txt' 
% and the 7'th entry in '../stellarStructure/io/SC/SC.txt'
%
% bSys = binarySystem(starSetName, starIndex)

function bSys = binarySystem(starSetName, starIndex)

	constants;
  global binseisToolboxPath;
  
  starSetInfo = load([starSetName, '.txt']);
  M = starSetInfo(starIndex, 1);
  D = starSetInfo(starIndex, 2);

  stellarStructureFile = ...
    [starSetName, sprintf('%02d', starIndex), '.txt'];
  
 	bSys.sst = loadStellarStructure(stellarStructureFile, true);
  
	load([binseisToolboxPath,'data/tables/kt.mat']);
  bSys.kt = resampleKtFuncs(eir, eipsi);
  
	if D == -1 % single star case

		bSys.D = -1;
		bSys.M1 = M * M_Sun;
		bSys.M2 = NaN;

		bSys.omegaOrbit = 0;
		bSys.L1 = NaN;
		bSys.r1 = [0,0,0];
		bSys.r2 = [NaN,NaN,NaN];

    bSys.Rbound = NaN;
    bSys.Lsmooth = NaN;

	else % double star case

		bSys.D = 2 * D * R_Sun; % binarySystem's D is twice stellarStructure's D
		bSys.M1 = M * M_Sun;
		bSys.M2 = M * M_Sun;

		bSys = buildBinarySystem(bSys);

    bSys.Rbound = 0.65 * bSys.sst.R(end);
    bSys.Lsmooth = bSys.Rbound / 6;
  end

%
% kt = resampleKtFuncs(eir, eipsi)
%
% remove NaN grid points at lower radiuses and interpolate eir, eipsi to
% obtain a regular spaced r(psi)
  
function kt = resampleKtFuncs(eir, eipsi)

	eir = eir(24:end);
	eipsi = eipsi(24:end);

  psiStep = (eipsi(end) - eipsi(end-1)) / 4;
  kt.psi = eipsi(1):psiStep:eipsi(end);
  kt.r = interp1(eipsi, eir, kt.psi, 'cubic');
  
%
% bSys = buildBinarySystem(bSys)

function bSys = buildBinarySystem(bSys)

	global G;

	D1 = (bSys.D * bSys.M2) / (bSys.M1 + bSys.M2); % Find D1, D2 such that the COM is at 0
	D2 = bSys.D - D1;
	bSys.r1 = [-D1,0,0]; 
	bSys.r2 = [+D2,0,0];

	bSys.omegaOrbit = sqrt((G * bSys.M2) / (D1 * bSys.D^2)); % Find orbital angular frequency

	bSys.L1 = ... % Find L1
		firstLagrangePoint(bSys.M1, bSys.M2, D1, D2, bSys.omegaOrbit);

%
% Numerically solve for the first lagrange point by finding a
% root to the force-x eq. along the x axis between -D1 and D2

function L1 = firstLagrangePoint(M1, M2, D1, D2, omegaOrbit)

	global G;
  global R_Sun;
  
	x = (-D1):R_Sun * 1e-5:D2;
	eqLHS = (omegaOrbit^2) * x + G*(-M1 ./ (x+D1).^2 + M2 ./ (x-D2).^2);
	[minV, minI] = min(abs(eqLHS));
	L1 = x(minI);
