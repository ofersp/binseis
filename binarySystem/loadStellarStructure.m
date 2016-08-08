%
% LOADSTELLARSTRUCTURE Load a stellar structure file into an sst structure.
%
% sst = loadStellarStructure(fname, resample)

function sst = loadStellarStructure(fname, resample)

	% load stellarStructure's ascii output, remove the I field and place
	% the R field at the top

	S = load(fname);	
	S = S(:,2:end);
	M = S(:,1);
	S(:,1) = S(:,2);
	S(:,2) = M;

	numTotalShells = size(S,1);
	numPhotosphereShells = (numTotalShells + 1) / 5;
  if mod((numTotalShells + 1), 5) ~= 0
		error('numTotalShells + 1 must be a multiple of 5'); end

	bpsP = S(end - numPhotosphereShells + 1, 5);
	bpsRho = S(end - numPhotosphereShells + 1, 7);
	bpsGamma1 = S(end - numPhotosphereShells + 1, 14);
	bpsVs = sqrt(bpsGamma1 * bpsP / bpsRho);

	sst.basePhotosphereP = bpsP;
	sst.basePhotosphereVs = bpsVs;

	if resample
		[S, sst.resBoundR, sst.resBoundI] = resampleStellarStructure(S);
  else
    sst.resBoundR = NaN;
    sst.resBoundI = NaN;
  end

	i = 1;
	sst.R = S(:,i); i = i+1;
	sst.M = S(:,i); i = i+1;
	sst.T = S(:,i); i = i+1;
	sst.L = S(:,i); i = i+1;
	sst.P = S(:,i); i = i+1;
	sst.Pg = S(:,i); i = i+1;
	sst.rho = S(:,i); i = i+1;
	sst.kappa = S(:,i); i = i+1;
	sst.epsilon = S(:,i); i = i+1;
	sst.epsilonPP = S(:,i); i = i+1;
	sst.epsilonCNO = S(:,i); i = i+1;
	sst.epsilonHe = S(:,i); i = i+1;
	sst.Cv = S(:,i); i = i+1;  
	sst.Gamma1 = S(:,i); i = i+1;
	sst.Gamma2 = S(:,i); i = i+1;
	sst.ChiT = S(:,i); i = i+1;
	sst.ChiRho = S(:,i); i = i+1;
	sst.delta = S(:,i); i = i+1;
	sst.Cp = S(:,i); i = i+1;
	sst.mu = S(:,i); i = i+1;
	sst.fp = S(:,i); i = i+1;
	sst.ft = S(:,i); i = i+1;
	sst.S = S(:,i); i = i+1;
	sst.avgG1 = S(:,i); i = i+1;
	sst.avgG2 = S(:,i); i = i+1;
	sst.delRad = S(:,i); i = i+1;
	sst.delAd = S(:,i); i = i+1;
	sst.del = S(:,i); i = i+1;
	sst.Lconv = S(:,i); i = i+1;
	sst.Lrad = S(:,i); i = i+1;
	i = i+1; % dummy field here
	sst.avgChiMLT = S(:,i); i = i+1;
	sst.avgSqrVc = S(:,i); i = i+1;

% resample loadStellarStructure()'s intermediate format at two spatial
% resolutions (with a higher resolution in the outer 5%)

function [S2, resBoundR, resBoundI] = resampleStellarStructure(S1)  

	R1 = S1(:,1);
  
  % shift the innermost grid point to the origin. the resulting kink is
  % later smoothed out by the cubic interpolation
  
  R1(1) = 0; 
  
  resBoundR = R1(end) * 0.95;
	R2stepOuter = (R1(end) - R1(end-1)) / 2;
  R2stepInner = 40 * R2stepOuter;
  
  % fix R2stepInner and R2stepOuter so that there is an integral number of
  % them in the inner and outer ranges

  Nouter = round((R1(end) - resBoundR) / R2stepOuter);  
  Ninner = round((resBoundR - 0) / R2stepInner);
	R2stepOuter = (R1(end) - resBoundR) / Nouter;
  R2stepInner = (resBoundR - 0) / Ninner;
  
	R2inner = [0:R2stepInner:resBoundR]';
  R2outer = [resBoundR + R2stepOuter:R2stepOuter:R1(end)]';  
  R2 = [R2inner; R2outer];
  
  resBoundI = numel(R2inner) - 1;
  
	S2 = zeros(size(R2,1),size(S1,2));
	S2(:,1) = R2;

	for i=2:size(S1,2)
		S2(:,i) = interp1(R1, S1(:,i), R2, 'cubic'); 
  end
