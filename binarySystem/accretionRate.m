%
% ACCRETIONRATE Estimate the accretion rate of a given binary system. 
%
% Resulting Mdot is in kg/sec.
%
% Mdot = accretionRate(bSys)

function Mdot = accretionRate(bSys)

	R = bSys.sst.R(end);
	spaceStep = R / 250;
	constants;

	C1 = [bSys.L1, -R/4, -R/4];
	C2 = [bSys.L1,  R/4,  R/4];
	[X, Y, Z] = coordsGrid(C1, C2, spaceStep);

	T =  squeeze(sampleStructure(bSys, bSys.sst.T, X, Y, Z));
	rho =  squeeze(sampleStructure(bSys, bSys.sst.rho, X, Y, Z));
	T(isnan(T)) = 0;
	rho(isnan(rho)) = 0;
	
	rmsVx = sqrt((k_B / m_H) * T);
	massFlux = (rho / 2) .* rmsVx;
	Mdot = (spaceStep^2) * sum(massFlux(:));
	
