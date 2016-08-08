
#include <math.h>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>

#include <interpolate.h>
#include <constants.h>

#include "rayTracer.h"


void RayTracer::init(const BinarySystem *bSys) { 

	this->bSys = bSys; 

	epsR = 1;	
	drx = Vec3d(epsR, 0, 0);
	dry = Vec3d(0, epsR, 0);
	drz = Vec3d(0, 0, epsR);

	epsK = 1e-10;
	dkx = Vec3d(epsK, 0, 0);
	dky = Vec3d(0, epsK, 0);
	dkz = Vec3d(0, 0, epsK);
}


void RayTracer::trace(Ray& ray) const {

	Vec3d dr, dk;
	bool encounteredNeg = false;
	bool encounteredNaN = false;
	bool outOfBounds = false;
	int i = 0, j = 0;	

	Vec3d r = ray.r0;
	Vec3d k = ray.k0;

	for(; i < ray.maxSteps &&
		!encounteredNeg && !encounteredNaN && !outOfBounds; i++) {

		double newOmega = traceStepRK4(r, k, ray.branch, ray.timeStep);

		if(i % ray.sampleEach == 0){

  		ray.path[j].r = r;
			ray.path[j].k = k;
			ray.path[j].omega = newOmega;      
      ray.path[j].atSurface = atSurface(r, k, newOmega);
			ray.surfaceSamples += (int)(ray.path[j].atSurface > 0.0);
			j++;
		}

		encounteredNeg = newOmega < 0;
		encounteredNaN = isnan(newOmega);
		outOfBounds = r.x >= bSys->L1;
	}

	ray.simSamples = j;

	if(outOfBounds) ray.stopCond = Ray::outOfBounds;
	else if(encounteredNaN) ray.stopCond = Ray::encounteredNaN;
	else if(encounteredNeg) ray.stopCond = Ray::encounteredNeg;
	else ray.stopCond = Ray::completedRun;

	// check if the last sample should be removed

	if(ray.stopCond != Ray::completedRun) {

		ray.simSamples--;
		if(ray.path[ray.simSamples].atSurface > 0)
			ray.surfaceSamples--;
	}
}


inline void RayTracer::smoothingWeigths(double R1, double 
	&innerW, double &outerW) const {

	outerW = (1 + erf((R1 - bSys->Rbound) / bSys->Lsmooth)) / 2;
	innerW = 1 - outerW;
}


inline bool RayTracer::sampleKT(double psiTilde, double &rTilde) const {
	
	if(psiTilde < bSys->kt.psi0 || psiTilde > bSys->kt.psi1)
		return false; // sample attempted beyond psi bounds

	double i = ((psiTilde - bSys->kt.psi0) * (bSys->kt.size-1) / 
		(bSys->kt.psi1 - bSys->kt.psi0));

  rTilde = interpScalarArray(bSys->kt.r, i);
	return true;
}


inline bool RayTracer::sampleSST(double *F, double r, double &f) const {

	if(r < bSys->sst.R0 || r > bSys->sst.R1)
		return false; // sample attempted beyond sst bounds

	double i;
	double numLower = bSys->sst.resBoundI + 1;
	double numUpper = bSys->sst.size - numLower;

	if(r < bSys->sst.resBoundR) { // inner resolution portion of sst

		i = bSys->sst.resBoundI * (r - bSys->sst.R0) / 
			(bSys->sst.resBoundR - bSys->sst.R0);
	} 
	else { // outer resolution portion of sst

		i = numUpper * (r - bSys->sst.resBoundR) / 
			(bSys->sst.R1 - bSys->sst.resBoundR) + bSys->sst.resBoundI;
	}
		
	f = interpScalarArray(F, i);
	return true;
}


bool RayTracer::sampleG(const Vec3d &r, double &g, Vec3d &gHat) const {

	const double epsG = 1e-5;
	const double epsR = 100.0;
	bool rv = true;

	Vec3d p1 = r - bSys->r1;
	double avgG1, R1 = sqrt(p1 * p1);
	rv &= sampleSST(bSys->sst.avgG1, R1, avgG1);

	if(bSys->D < 0) { // single star case
		
		g = -avgG1;
		gHat = p1 * (-1.0 / R1);
	}
	else { // double star case

		Vec3d p2 = r - bSys->r2;
		double R2 = sqrt(p2 * p2);
		Vec3d rxy(r.x, r.y, 0);

		Vec3d g1out = (R1 > epsR) ? p1 * ((-G*bSys->M1) / pow(R1, 3)) : 0.0;
		Vec3d g1int = rv ? p1 * (-avgG1 / R1) : g1out;
		rv = true;

		Vec3d g2 = p2 * ((-G*bSys->M1) / pow(R2, 3));
		Vec3d cent = rxy * (bSys->omegaOrbit * bSys->omegaOrbit);

		// combine g1int and g1out

		double innerW, outerW;
		smoothingWeigths(R1, innerW, outerW);
		Vec3d g1 = g1int * innerW + g1out * outerW;

		Vec3d gVec = g1 + g2 + cent;
		g = sqrt(gVec * gVec);
		gHat = g > epsG ? gVec / g : Vec3d(1,0,0);
	}
	return rv;
}


double RayTracer::samplePsiExternal(const Vec3d &r) const {

	const double epsR = 100.0;
	double psi;

	Vec3d p1 = r - bSys->r1;
	Vec3d p2 = r - bSys->r2;
	double R1 = sqrt(p1 * p1);	
	double R2 = sqrt(p2 * p2);
	double RxySqr = (r.x * r.x) + (r.y * r.y);

	if(R1 > epsR && R2 > epsR) {

		double centripetalPart = RxySqr * (bSys->omegaOrbit * bSys->omegaOrbit) / -2.0;
		double gravPart = -G * (bSys->M1 / R1 + bSys->M2 / R2);
		psi = centripetalPart + gravPart;
	}
	return psi;
}


bool RayTracer::sampleStructure(const Vec3d &r, double &P, 
	double &rho, double &Gamma1) const {

	bool rv = true;
	Vec3d p1 = r - bSys->r1;
	double Rcomb, R1 = sqrt(p1 * p1);

	if(bSys->D < 0) { Rcomb = R1; } // single star case

	else { // double star case

		double rTilde = R1 / (bSys->D / 2.0);
		double psi = samplePsiExternal(r);
    double alpha = G * bSys->M1 / (bSys->D / 2.0);
		sampleKT(psi / alpha, rTilde) ;		
		double R1psi = rTilde * (bSys->D / 2.0); 

		// combine R1 and R1psi

		double innerW, outerW;
		smoothingWeigths(R1, innerW, outerW);
    Rcomb = innerW * R1 + outerW * R1psi;
	}

	// sample structure at Rcomb

	rv &= sampleSST(bSys->sst.P, Rcomb, P);
	rv &= sampleSST(bSys->sst.rho, Rcomb, rho);
	rv &= sampleSST(bSys->sst.Gamma1, Rcomb, Gamma1);

	return rv;
}


double RayTracer::omega(const Vec3d& r, const Vec3d& k, double branch) const {

	Vec3d gHat;	
	double P, rho, Gamma1, g;
	bool rv = true;

	rv &= sampleStructure(r, P, rho, Gamma1);
	rv &= sampleG(r, g, gHat);

	double aSqr = Gamma1 * (P / rho);
	double omegaASqr = ((Gamma1*g) * (Gamma1*g)) / (4.0 * aSqr);

	return rv ? omegaGough(k, omegaASqr, aSqr)  : -1.0;
}
 

/** compute the local dispersion relation omega(k) */

double RayTracer::omegaGough(const Vec3d& k, double omegaASqr, double aSqr) 
  const {
          
	double kSqr = k * k;
	double omegaSqr = omegaASqr + aSqr*kSqr;

	return sqrt(omegaSqr);	
}


double RayTracer::omegaIsothermal(const Vec3d& k, double branch, 
	const Vec3d& gHat, double omegaASqr, double aSqr, double Gamma1) const {
          
	double kSqr = k * k;
	double k2 = k * gHat;
	double k1 = sqrt(fabs(kSqr - k2*k2));

	double omegaGSqrOverOmegaASqr = (Gamma1 - 1) / (Gamma1 * Gamma1);
	double omegaGSqr = omegaASqr * omegaGSqrOverOmegaASqr;

	double A = omegaASqr + aSqr * kSqr;
	double tmp = A*A - 4 * omegaGSqr * aSqr * k1*k1;
	double omegaSqr = 0.5 * (A + branch*sqrt(tmp));

	return sqrt(omegaSqr);	
}


/**
 * A ray is defined to be at the surface if it is one wavelength away from 
 * the photosphere's base or above that point but still within the star.
 *
 * @return 1.0 if at the stellar surface, 0.0 if not and -1.0 if unknown
 */

double RayTracer::atSurface(const Vec3d &r, const Vec3d &k, double omega) const {
  
	double bpsLambda = two_pi * bSys->sst.basePhotosphereVs / omega;
	double bpsP = bSys->sst.basePhotosphereP;

  double kMag = sqrt(k*k);
  if(kMag < epsK) // kMag too small to determine kHat
    return -1.0;

  Vec3d kHat = k / kMag;
  Vec3d dr = kHat * bpsLambda;
  
  // sample at rf and rb attempting to detect the surface
  
  Vec3d rf = r + dr;
  Vec3d rb = r - dr;
 
  double Pf, Pb, dummy;
  bool rvf = sampleStructure(rf, Pf, dummy, dummy);
  bool rvb = sampleStructure(rb, Pb, dummy, dummy);

	// if either rf or rb are out of bounds or if the pressure at rf or rb is 
	// below bpsP then r is at the surface

  if(!rvf || !rvb || Pf < bpsP || Pb < bpsP) return 1.0;  
  return 0.0;
}

