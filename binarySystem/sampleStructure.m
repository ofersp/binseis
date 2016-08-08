%
% SAMPLESTRUCTURE Sample scalar field in given bSys at positions X,Y,Z.
%
% [F, psi] = sampleStructure(bSys, Fr, X, Y, Z)

function [F, psi] = sampleStructure(bSys, Fr, X, Y, Z)

	[R1, R2, Rxy] = coordsBinary(bSys.r1, bSys.r2, X, Y, Z);
  clear R2 Rxy;
  
  psi = samplePsiExternal(bSys.r1, bSys.r2, bSys.M1, bSys.M2, ...
    bSys.omegaOrbit, X, Y, Z);
  
  if bSys.D == -1.0 
    
    Rcomb = R1; % single star case
    
  else % double star case    

	  global G;
    alpha = G * bSys.M1 / (bSys.D / 2);
    R1psi = interp1(bSys.kt.psi, bSys.kt.r, psi(:) / alpha) * (bSys.D/2);
    R1psi(find(isnan(R1psi))) = -1.0;
    R1psi = reshape(R1psi, size(X));

    outerK = (1 + erf((R1 - bSys.Rbound) / bSys.Lsmooth)) / 2;
    innerK = 1 - outerK;
    Rcomb = innerK.*R1 + outerK.*R1psi;
  end
  
  F = interp1(bSys.sst.R, Fr, Rcomb);
  F = reshape(F, size(X));
