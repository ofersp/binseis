#ifndef STRUCTURE_SCALARS_H
#define STRUCTURE_SCALARS_H

class StructureScalars {

public:

	double M, R, T, L, P, Pg;
	double rho, kappa, epsilon, epsilonPP, epsilonCNO, epsilonHe;
	double Cv, Gamma1, Gamma2, ChiT, ChiRho, delta, Cp, mu;
	double fp, ft, S, avgG1, avgG2, delRad, delAd, del, Lconv, Lrad;
	double avgChiMLT, dummy, avgSqrVc;

	enum{numFields = 33};	

	void print();
};

#endif // STRUCTURE_SCALARS_H
