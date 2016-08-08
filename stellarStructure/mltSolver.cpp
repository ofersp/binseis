
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

/**
 Code pieces for a solver of the MLT cubic equation. Converted from the 
 fortran version mlt_solver.f found here -
 http://www.mpa-garching.mpg.de/stars/SCC/Tools.html)

 This version of MLT is exactly that of Kippenhahn/Weigert's
 textbook "Stellar Structure and Evolution" we solve their equation 7.12 
 with a simple Newton solver for a cubic equation, which is coded here 
 as an internal function ("gmlt"), the derivatvie of which w.r.t the 
 variable "ax" is "dgmlt". "ax" corresponds to Kippenhahn's quantity xsi
 (his formula 7.17), and is called "xnab" in this coding.
 His quantity "U" is here "UNAM", and from this and the adiabatic gradient 
 "adim" the convective gradient "xsol" can be computed. The starting value 
 for the iteration is always the mean between the radiative and the adiabatic
 gradient. */

// Newton iteration parameters

const int mnit = 50;
const double under = 1.0;
const double acc = 1e-4;

// Geometrical factors of MLT equation

const double c1 = 19.0 / 9.0;
const double c2 = 17.0 / 9.0;
const double c3 = 8.0 / 9.0;
const double c4 = 3.0e0;

inline double gmlt(double ax, double a1, double a2, double a3) {

	return (ax*ax*ax) + a1*(ax*ax) + a2*ax + a3;
}

inline double dgmlt(double ax, double a1, double a2) { 

	return 3.0*(ax*ax) + 2.0*a1*ax + a2;
}

inline double der3(double ax, double a1, double a2, double a3, 
	double b1, double b2, double b3) {

	return -(ax*ax)*c1*b3 + 2.0*ax*c4*a3*b3 - 
		3.0*(a3*a3)*b3*c2 - b3*c3*(a2 - a1) - a3*c3*(b2 - b1);
}

/** Solve the cubic equation of MLT -
 radm and adim are the radiative and adiabatic gradients, 
 unam is the quantity U of eq. 7.12 of Kippenhahn and Weigert's textbook */

double mltSolver(double radm, double adim, double unam) {

	double xnab = sqrt((radm - adim)/2 + (unam*unam));
	double xco1 = -unam*c1;
	double xco2 = c4*(unam*unam);
	double xco3 = -unam*((unam*unam)*c2 + c3*(radm - adim));

	// a simple Newton solver -

	int i = 1; 
	double dxnab;

	for(; i <= mnit; i++) {
    
	  dxnab = -gmlt(xnab,xco1,xco2,xco3) / dgmlt(xnab,xco1,xco2);
		xnab = xnab + dxnab*under;

		// simple criterion for convergence -
	  if(fabs(dxnab) < acc) break;
	}

	if(i == mnit) {

		printf("no solution for MLT equation: ");
		printf("radm = %f, adim = %f, unam = %f, xnab = %f, dxnab = %f\n",
			radm, adim, unam, xnab, dxnab);
		exit(1);
	}

	double resid = fabs(gmlt(xnab,xco1,xco2,xco3));

	if(resid > acc) {
		printf("residuum too large: xnab = %f, resid = %f\n", xnab, resid);
		exit(2);
	}

	double xsol = (xnab*xnab) - (unam*unam) + adim;
	return xsol;
}

