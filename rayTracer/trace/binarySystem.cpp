
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include "binarySystem.h"


bool BinarySystem::read(const char *fname) {

	assert(sst.P == NULL);

	bool rv = true;
	FILE *fp = fopen(fname, "rb");
	if(!fp) return false;

	rv &= fread(&D, sizeof(D), 1, fp) == 1;
	rv &= fread(&M1, sizeof(M1), 1, fp) == 1;
	rv &= fread(&M2, sizeof(M2), 1, fp) == 1;
	rv &= fread(&r1, sizeof(r1), 1, fp) == 1;
	rv &= fread(&r2, sizeof(r2), 1, fp) == 1;
	rv &= fread(&omegaOrbit, sizeof(omegaOrbit), 1, fp) == 1;
	rv &= fread(&L1, sizeof(L1), 1, fp) == 1;
	rv &= fread(&Rbound, sizeof(Rbound), 1, fp) == 1;
	rv &= fread(&Lsmooth, sizeof(Lsmooth), 1, fp) == 1;

	rv &= fread(&sst.R0, sizeof(sst.R0), 1, fp) == 1;
	rv &= fread(&sst.R1, sizeof(sst.R1), 1, fp) == 1;
	rv &= fread(&sst.resBoundR, sizeof(sst.resBoundR), 1, fp) == 1;
	rv &= fread(&sst.resBoundI, sizeof(sst.resBoundI), 1, fp) == 1;
  rv &= fread(&sst.basePhotosphereP, sizeof(sst.basePhotosphereP), 1, fp) == 1;
	rv &= fread(&sst.basePhotosphereVs, sizeof(sst.basePhotosphereVs), 1, fp) == 1;
	rv &= fread(&sst.size, sizeof(sst.size), 1, fp) == 1;

	sst.P = (double*)calloc(sst.size, sizeof(double));
	sst.rho = (double*)calloc(sst.size, sizeof(double));
	sst.Gamma1 = (double*)calloc(sst.size, sizeof(double));
	sst.avgG1 = (double*)calloc(sst.size, sizeof(double));

	rv &= sst.P != NULL;
	rv &= sst.rho != NULL;
	rv &= sst.Gamma1 != NULL;
	rv &= sst.avgG1 != NULL;

	rv &= fread(sst.P, sizeof(double), sst.size, fp) == sst.size;
	rv &= fread(sst.rho, sizeof(double), sst.size, fp) == sst.size;
	rv &= fread(sst.Gamma1, sizeof(double), sst.size, fp) == sst.size;
	rv &= fread(sst.avgG1, sizeof(double), sst.size, fp) == sst.size;

	rv &= fread(&kt.psi0, sizeof(kt.psi0), 1, fp) == 1;
	rv &= fread(&kt.psi1, sizeof(kt.psi1), 1, fp) == 1;
	rv &= fread(&kt.size, sizeof(kt.size), 1, fp) == 1;

	kt.r = (double*)calloc(kt.size, sizeof(double));
	rv &= kt.r != NULL;
	rv &= fread(kt.r, sizeof(double), kt.size, fp) == kt.size;

	return rv;
}

