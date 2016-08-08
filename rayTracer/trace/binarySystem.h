
#ifndef BINARY_SYSTEM_H
#define BINARY_SYSTEM_H

#include <vec3.h>


class BinarySystem {

public:

	BinarySystem() {

		sst.P = NULL;
		sst.rho = NULL;
		sst.Gamma1 = NULL;
		sst.avgG1 = NULL;
		kt.r = NULL;
	}

	~BinarySystem() {
		
		if(sst.P) free(sst.P);
		if(sst.rho) free(sst.rho);
		if(sst.Gamma1) free(sst.Gamma1);
		if(sst.avgG1) free(sst.avgG1);
		if(kt.r) free(kt.r);
	}

	bool read(const char *fname);

public:

	typedef struct {

		double R0, R1;
		double resBoundR;
		int resBoundI;
    double basePhotosphereP;
    double basePhotosphereVs;
		int size;
		double *P;
		double *rho;
		double *Gamma1;
		double *avgG1;
	} StarStructure;

	typedef struct {

		double psi0, psi1;
		int size;
		double *r;
	} KtFuncs;

	double D;
	double M1, M2;
	Vec3d r1,r2;
	double omegaOrbit;
	double L1;
	double Rbound;
	double Lsmooth;	
	StarStructure sst;
	KtFuncs kt;
};

#endif
