%
% SAMPLEPSIEXTERNAL Sample effective potential given binary system params.
%
% psi = samplePsiExternal(r1, r2, M1, M2, omegaOrbit, X, Y, Z)

function psi = samplePsiExternal(r1, r2, M1, M2, omegaOrbit, X, Y, Z)

	[R1, R2, Rxy] = coordsBinary(r1, r2, X, Y, Z);
	psi = psiExternal(M1, M2, omegaOrbit, R1, R2, Rxy);

%
% psiExternal computes the star potential in the external point potential
% domain. M1 and M2 are the star masses. R1 and R2 are the distances
% between the star centers and the points of evaluation. Rxy are the distances
% to the CM in the xy plane. 
%
% psi = psiExternal(M1, M2, omegaOrbit, R1, R2, Rxy)

function psi = psiExternal(M1, M2, omegaOrbit, R1, R2, Rxy)

	global G;  
  centripetalPart = -(Rxy.^2 .* (omegaOrbit^2)) / 2;
	gravPart = -G * (M1 ./ R1 + M2 ./ R2);
	psi = centripetalPart + gravPart;
	psi(find(isinf(psi))) = 0;
