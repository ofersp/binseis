%
% SAMPLEG Sample effective (grav + cent) accel at X,Y,Z in given bSys.
%
% [g, gHat] = sampleG(bSys, X, Y, Z)

function [g, gHat] = sampleG(bSys, X, Y, Z)	

	numSamples = numel(X);
	Pos = [X(:) Y(:) Z(:)]; 
	PosRelR1 = Pos - ones(numSamples, 1) * bSys.r1;
	R1 = sqrt(sum(PosRelR1.^2, 2));	
	
	if bSys.D == -1 % single star case

		g = sampleStructure(bSys, bSys.sst.avgG1, X, Y, Z);
		gHat = -PosRelR1 ./ (R1 * [1 1 1]);

  else % double star case		

		global G;		
		PosRelR2 = Pos - ones(numSamples, 1) * bSys.r2;
		R2 = sqrt(sum(PosRelR2.^2, 2));
		PosXY = [X(:) Y(:) zeros(numSamples, 1)];		

		g1int = -((interp1(bSys.sst.R, bSys.sst.avgG1, R1) ./ R1) * [1 1 1]) .* PosRelR1;
    g1int(find(isnan(g1int))) = 0.0;
		g1out = -(((G*bSys.M1)./(R1.^3)) * [1 1 1]) .* PosRelR1;
		g2 = -(((G*bSys.M2)./(R2.^3)) * [1 1 1]) .* PosRelR2;
		cent = (bSys.omegaOrbit^2) * PosXY;

		outerK = ((1 + erf((R1 - bSys.Rbound) / bSys.Lsmooth)) / 2) * [1 1 1];
		innerK = 1 - outerK;
		g1 = innerK .* g1int + outerK .* g1out;
		
		gVec = g1 + g2 + cent;
		g = sqrt(sum(gVec.^2, 2));
		gHat = gVec ./ (g * [1 1 1]);
	end

	g = reshape(g, size(X));
	gHat = squeeze(reshape(gHat, [size(X) 3]));	
	
	if numel(g) == 1 % this is how solitary 3-vectors should be arranged
		gHat = gHat';
	end
	
