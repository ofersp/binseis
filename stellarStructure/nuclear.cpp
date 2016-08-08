
#include <constants.h>
#include "nuclear.h"


double nuclear(double T, double rho, double X, double Z, 
	double &epsilonPP, double &epsilonCNO, double &epsilonHe) {

	const double T0 = 1.3374599500e+07; // these were obtained from fitting EZ at T0
  const double nuPP  = 4.6080873075e+00; 
  const double nuCNO = 1.9386046316e+01;
  const double eps0PPcgs  = 1.7008818632e-01; 
  const double eps0CNOcgs = 6.1449468048e-02;
  
  double Tnorm = T/T0;
	double rhocgs = rho*1.0e-3;

  epsilonPP = eps0PPcgs*rhocgs*pow(Tnorm,nuPP)*1e-4;
  epsilonCNO = eps0CNOcgs*rhocgs*pow(Tnorm,nuCNO)*1e-4;
  epsilonHe = 0; // unimplemented
  
  double epsilon = epsilonPP + epsilonCNO + epsilonHe;
	return epsilon;
}
