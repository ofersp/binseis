
#include <assert.h>
#include <stdio.h>
#include <algorithm>

extern "C" {
#include <atlas/clapack.h>
}

#include "nuclear.h"
#include "mltSolver.h"
#include "photosphere.h"
#include "starBuilder.h"


StarBuilder::StarBuilder(
	int numShells, double M, 
	double Rs, double Ls, double Tc, double Pc, 
	double alphaMLT, double D, double X, double Z, 
	const char *eosTablesFname, const char *opalTableFname, 
	const char *ktTablesFname) {

	this->numShells = numShells;
	this->fittingShell = numShells / 2;
	this->numPhotosphereShells = numShells / 4;

	this->M = M;
	this->Rs = Rs;
	this->Ls = Ls;
	this->Tc = Tc;
	this->Pc = Pc;
	this->alphaMLT = alphaMLT;
	this->D = D;

	this->X = X;
	this->Z = Z;
	this->Y = 1 - X - Z;

	// allocate and initialize the structure-scalar array

	ssc = new StructureScalars[numShells];
	initMassShells();
	
	// load tables for the equation-of-state, opacity and Kippenhahn-Thomas functions

	assert(eosTables.load(eosTablesFname));
	assert(opalTable.load(opalTableFname));
	assert(ktTables.load(ktTablesFname, D, M));
}


void StarBuilder::initMassShells() {

	const double eta = 5e-10;
	const int numZones = 3;
	int zoneShells[] = {0.33 * numShells, 0, 0.33 * numShells};
	double zoneMass[] = {0.01 * M, 0, 0.33 * M};

	int i, leftoverShells = 0;
	double leftoverMass = 0;
	for(i = 0; i < numZones; i++) {
		leftoverShells += zoneShells[i];
		leftoverMass += zoneMass[i];
	}

	zoneShells[1] = numShells - leftoverShells;
	zoneMass[1] = M - leftoverMass;
	double zonedM[numZones - 1]; 
	for(i = 0; i < numZones - 1; i++)
		zonedM[i] = zoneMass[i] / zoneShells[i];

	// set inner linear zones

	int l = 0;
	double lastM = 0;

	for(int j = 0; j < numZones - 1; j++) {
		for(int k = 0; k < zoneShells[j]; k++) {
			ssc[l++].M = lastM + zonedM[j] * (k + 1);
		}
		lastM += zoneMass[j];
	}

	// set outer logarithmic (envelope) zone

	const double xia = log(1.0 - lastM/M);
	const double xib = log(eta);
	const double dxi = (xib - xia) / (zoneShells[2] - 1);	

	for(i = 0; i < zoneShells[2]; i++) {

		double xi = xia + dxi * (i + 1);
		ssc[l++].M = M * (1.0 - exp(xi));
	}
}


bool StarBuilder::sampleStructure(double M, double R, double T, double L, 
	double P, StructureScalars &s, bool inPhotosphere) {

	try {
		s.M = M;
		s.R = R;
		s.T = T;
		s.L = L;
		s.P = P;
		s.Pg = P - a_rad_o3 * pow(T, 4);
		s.rho = eosTables.rho(T, P);
		s.kappa = opalTable.opacity(T, s.rho);
		s.epsilon = nuclear(T, s.rho, X, Z, s.epsilonPP, s.epsilonCNO, s.epsilonHe);
		s.Cv = eosTables.Cv(T, P);
		s.Gamma1 = eosTables.Gamma1(T, P);
		s.Gamma2 = eosTables.Gamma2(T, P);
		s.ChiRho = eosTables.ChiRho(T, P);
		s.ChiT = eosTables.ChiT(T, P);
		s.delta = s.ChiRho / s.ChiT;
		s.Cp = s.Cv + (s.P/(s.rho*s.T))*(s.ChiT*s.ChiT)/s.ChiRho;
		s.mu = (k_B * s.rho * T) / (m_H * s.Pg);
		s.fp = D < 0 ? 1.0 : ktTables.fp(s.R);
		s.ft = D < 0 ? 1.0 : ktTables.ft(s.R);
		s.S = (four_pi*s.R*s.R) * (D < 0 ? 1.0 : ktTables.u(s.R));
		s.avgG1 = (G*M / (s.R*s.R)) * (D < 0 ? 1.0 : ktTables.v(s.R));
		s.avgG2 = pow(G*M / (s.R*s.R), 2) * (D < 0 ? 1.0 : ktTables.z(s.R));
		s.delRad = (s.ft/s.fp) * (3.0*s.kappa*s.L*s.P) / (16.0*pi*a_rad*c*pow(s.T,4)*G*s.M);
		s.delAd = (s.Gamma2 - 1) / s.Gamma2;

		bool isConv = inPhotosphere ? false : fabs(s.delRad) > fabs(s.delAd);
		double U = !isConv ? -1 :
			s.avgG1*(6.0*sqrt(2.0)*a_rad*c*pow(s.T,3.0)/sqrt(s.rho*s.delta*pow(s.P,3.0)))/
			(s.Cp*s.kappa*alphaMLT*alphaMLT);

		s.del = !isConv ? s.delRad : mltSolver(s.delRad, s.delAd, U);
		s.avgChiMLT = !isConv ? -1 : (8.0/9.0)*U*(s.delRad - s.del);
		s.Lconv = !isConv ? 0 :
			s.S*s.Cp*s.T*pow(alphaMLT,2)*sqrt(s.P*s.rho*s.delta)*s.avgChiMLT/(4*sqrt(2));
		s.Lrad = !isConv ? s.L : std::max(0.0, s.L - s.Lconv);
		s.avgSqrVc = !isConv ? 0 :
			s.delta*pow(alphaMLT, 2)*pow(s.avgChiMLT, 2.0/3.0)*s.P/(8.0*s.rho);
	}
	catch(int e) { return false; }

	// some sanity checks
	return s.M > 0 && s.R > 0 && s.T > 0 && s.L > 0 && s.P > 0 && s.Pg >= 0 &&
		s.rho >= 0 && s.kappa >= 0 && s.epsilon >= 0 &&
		s.Cv >= 0 && s.Gamma1 >= 0 && s.Gamma2 >= 0 && s.ChiRho >= 0 && s.ChiT >= 0 &&
		s.delta >= 0 && s.Cp >= 0 && s.mu >= 0 && s.fp >=0 && s.ft >= 0 && s.S >= 0 &&
		s.avgG1 >= 0 && s.avgG2 >= 0; // TODO: check all components of s
}


int StarBuilder::integrate(int shellStart, int shellStop, 
		double R0, double T0, double L0, double P0) {	

	const int numRKIters = 4;
	const int di = shellStart < shellStop ? 1 : -1;
	int i;

	// set initial values

	StructureScalars s;
	bool structureOK = 
		sampleStructure(ssc[shellStart].M, R0, T0, L0, P0, s, false);

	for(i = shellStart; i - di != shellStop && structureOK; i += di) {

		ssc[i] = s; // record current grid point
		if(i == shellStop) break;

		// perform some Runge-Kutta iterations to advance R, T, L and P

		double dM = ssc[i + di].M - ssc[i].M;
		double currdM = 0;
		double dRdM, Rl = s.R;
		double dTdM, Tl = s.T;
		double dLdM, Ll = s.L;
		double dPdM, Pl = s.P;

		for(int j = 0; j < numRKIters && structureOK; j++) {			

			// stellar structure equations

			dRdM = 1 / (four_pi * (s.R*s.R) * s.rho);
			dLdM = s.epsilon;
			dPdM = -s.fp * (G * s.M) / (four_pi * pow(s.R, 4));
			dTdM = (s.T/s.P) * dPdM * s.del;

			currdM = (j < (numRKIters - 1) ? dM / 2 : dM);

			s.R = Rl + dRdM * currdM;
			s.T = Tl + dTdM * currdM;
			s.L = Ll + dLdM * currdM;
			s.P = Pl + dPdM * currdM;
			s.M = ssc[i].M + currdM; 

			structureOK = sampleStructure(s.M, s.R, s.T, s.L, s.P, s, false);
		}
	}	
	return i-di;
}


int StarBuilder::integrateInwards(double Rs, double Ls, StructureScalars &rssc, bool findOverlap) {

	double surfaceArea = (four_pi * Rs*Rs) * (D < 0 ? 1.0 : ktTables.u(Rs));
	double Ts = pow(Ls / (surfaceArea * sigma), 1.0/4);
	double Ps = Photosphere::basalPressure(M, Rs, Ts, D, 
		eosTables, opalTable, ktTables, numPhotosphereShells, NULL);

	if (findOverlap)
		return integrate(numShells - 1, 0, Rs, Ts, Ls, Ps);

	integrate(numShells - 1, fittingShell, Rs, Ts, Ls, Ps);
	rssc = ssc[fittingShell];
	return -1;
}


int StarBuilder::integrateOutwards(double Tc, double Pc, StructureScalars &rssc, bool findOverlap) {

	double dummy1, dummy2, dummy3;
	double rhoc = eosTables.rho(Tc, Pc);
	double epsilonc = nuclear(Tc, rhoc, X, Z, dummy1, dummy2, dummy3);	

	double Rc = pow((1.0 / four_pi_o3) * ssc[0].M / rhoc, 1.0/3);
	double Lc = ssc[0].M * epsilonc;

	if (findOverlap)
		return integrate(0, numShells - 1, Rc, Tc, Lc, Pc);

	integrate(0, fittingShell, Rc, Tc, Lc, Pc);
	rssc = ssc[fittingShell];
	return -1;
}


void StarBuilder::setFittingShell(double Rs, double Tc, double Ls, double Pc) {

	StructureScalars dummySSC;	
	int lastGoodIn = integrateInwards(Rs, Ls, dummySSC, true);
	int lastGoodOut = integrateOutwards(Tc, Pc, dummySSC, true);
	assert(lastGoodIn <= lastGoodOut);
	fittingShell = (lastGoodIn + lastGoodOut) / 2;
}


void StarBuilder::build() {

	StructureScalars dummySSC;
	setFittingShell(Rs, Tc, Ls, Pc);
	integrateOutwards(Tc, Pc, dummySSC, false);
	integrateInwards(Rs, Ls, dummySSC, false);
}


void StarBuilder::print() { 
	for(int i = 0; i < numShells-1; i++)	{
		printf("%09d ", i);
		ssc[i].print();
	}	
}


bool StarBuilder::buildAndPrintPhotosphere() {

	bool structureOK = true;
	StructureScalars *sscPS = new StructureScalars[numPhotosphereShells];

	double M = ssc[numShells - 1].M;
	double R = ssc[numShells - 1].R;
	double T = ssc[numShells - 1].T;
	double L = ssc[numShells - 1].L;
	double P = ssc[numShells - 1].P;
	double rho = ssc[numShells - 1].rho;

	Photosphere::basalPressure(M, R, T, D, 
		eosTables, opalTable, ktTables, numPhotosphereShells, sscPS);

	for(int i = 0; i < numPhotosphereShells; i++) {

		double dRdM = 1 / (four_pi * (R*R) * rho);
		double dPdM = -ktTables.fp(R) * (G * M) / (four_pi * pow(R, 4));

		double dP = sscPS[i].P - P;
		double dM = dP / dPdM;

		M += dM;
		R += dRdM * dM;
		T = sscPS[i].T;
		// L = L;
		P = sscPS[i].P;
		rho = sscPS[i].rho;

		structureOK &= sampleStructure(M, R, T, L, P, sscPS[i], true);
		printf("%09d ", numShells + i - 1);
		sscPS[i].print();
	}
	return structureOK;
}


void StarBuilder::fit() {

	// start with given initial guess for the following boundary conditions

	double Rs = this->Rs;
	double Tc = this->Tc; 
	double Ls = this->Ls;
	double Pc = this->Pc;

	while(true) {

		double dRs = 1e-4 * Rs;
		double dTc = 1e-4 * Tc;
		double dLs = 1e-4 * Ls;
		double dPc = 1e-4 * Pc;

		setFittingShell(Rs, Tc, Ls, Pc);
		printf("%7e %7e %7e %7e\n", Rs/R_Sun, Tc, Ls/L_Sun, Pc);

		// integrate outwards

		StructureScalars sscTc; integrateOutwards(Tc + dTc, Pc, sscTc, false);
		StructureScalars sscPc; integrateOutwards(Tc, Pc + dPc, sscPc, false);		
		StructureScalars ssc0c; integrateOutwards(Tc, Pc, ssc0c, false);	

		// integrate inwards
	
		StructureScalars sscRs; integrateInwards(Rs + dRs, Ls, sscRs, false);
		StructureScalars sscLs; integrateInwards(Rs, Ls + dLs, sscLs, false);
		StructureScalars ssc0s; integrateInwards(Rs, Ls, ssc0s, false);

		// arrange the matrix of partial derivatives

		double dXdY[] = {
			(sscRs.R-ssc0s.R)/dRs, -(sscTc.R-ssc0c.R)/dTc, (sscLs.R-ssc0s.R)/dLs, -(sscPc.R-ssc0c.R)/dPc,
			(sscRs.T-ssc0s.T)/dRs, -(sscTc.T-ssc0c.T)/dTc, (sscLs.T-ssc0s.T)/dLs, -(sscPc.T-ssc0c.T)/dPc,
			(sscRs.L-ssc0s.L)/dRs, -(sscTc.L-ssc0c.L)/dTc, (sscLs.L-ssc0s.L)/dLs, -(sscPc.L-ssc0c.L)/dPc,
			(sscRs.P-ssc0s.P)/dRs, -(sscTc.P-ssc0c.P)/dTc, (sscLs.P-ssc0s.P)/dLs, -(sscPc.P-ssc0c.P)/dPc };
		
		// define target fitting conditions

		double targetDeltaRTLP[] = {
			-(ssc0s.R - ssc0c.R),
			-(ssc0s.T - ssc0c.T),
			-(ssc0s.L - ssc0c.L),
			-(ssc0s.P - ssc0c.P)};

		// update boundary conditions

		int ipiv[4];
		int rv = clapack_dgesv(CblasRowMajor, 4, 1, dXdY, 4, ipiv, targetDeltaRTLP, 4); 

		double rate = 0.3;

		double reldRs = fabs(targetDeltaRTLP[0] / Rs);
		double reldTc = fabs(targetDeltaRTLP[1] / Tc);
		double reldLs = fabs(targetDeltaRTLP[2] / Ls);
		double reldPc = fabs(targetDeltaRTLP[3] / Pc);

		double maxRelChange = std::max(std::max(reldRs, reldTc), std::max(reldLs, reldPc));
		if(maxRelChange > 1) rate /= maxRelChange;

		Rs += targetDeltaRTLP[0] * rate;
		Tc += targetDeltaRTLP[1] * rate;
		Ls += targetDeltaRTLP[2] * rate;
		Pc += targetDeltaRTLP[3] * rate;
	}
}
