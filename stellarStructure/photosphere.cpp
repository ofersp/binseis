
#include <constants.h>
#include "photosphere.h"


inline double Photosphere::temperature(double tau, double Te) {
	return pow((3.0/4) * pow(Te,4) * (tau + 2.0/3.0), 1.0/4);
}

double Photosphere::basalPressure(
	double Me, double Re, double Te, double D,
	EosTables &eosTables, OpalTable &opalTable, KtTables &ktTables, 
	int numShells, StructureScalars *ssc) {

	const double v =  D < 0 ? 1.0 : ktTables.v(Re);
	const double avgG = (G*Me / (Re*Re)) * v;
	const double taue = 2.0/3.0;
	const double dtau = taue / (numShells - 1);
	const double Ts = temperature(0, Te);

	// here we choose Pg > 0 yet much smaller than Pr so that rho is still 
	// finite
	const double Ps = 0.1 + (a_rad/6) * pow(Ts, 4);
  
	double P = Ps;	
	for(int i = 0; i < numShells; i++) {

		double rho, kappa;
		double tau = dtau * i;
		double T = temperature(tau, Te);

		rho = eosTables.rho(T, P);
		kappa = opalTable.opacity(T, rho);
		double dP1 = dtau / (kappa/avgG);
		rho = eosTables.rho(T, P + dP1 / 2);
		kappa = opalTable.opacity(T, rho);
		double dP2 = dtau / (kappa/avgG);
		rho = eosTables.rho(T, P + dP2 / 2);
		kappa = opalTable.opacity(T, rho);
		double dP3 = dtau / (kappa/avgG);
		rho = eosTables.rho(T, P + dP3);
		kappa = opalTable.opacity(T, rho);
		double dP4 = dtau / (kappa/avgG);

		P += dP1/6 + dP2/3 + dP3/3 + dP4/6;

		if(ssc) {
			ssc[numShells - i - 1].T = T;
			ssc[numShells - i - 1].P = P;
			ssc[numShells - i - 1].rho = rho;
		}
	}
	return P;
}

