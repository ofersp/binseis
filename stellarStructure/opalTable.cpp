#include <math.h>
#include <stdio.h>
#include <assert.h>

#include <interpolate.h>
#include "opalTable.h"


bool OpalTable::interpTable(double logT, double logR, double &logKappa) {

	double indLogT = ((logT - minLogT) / (maxLogT - minLogT)) * (numLogT - 1);
	double indLogR = ((logR - minLogR) / (maxLogR - minLogR)) * (numLogR - 1);

	if(indLogT < 0 || indLogT > numLogT - 1) return false;
	if(indLogR < 0 || indLogR > numLogR - 1) return false;

	logKappa = interp2dScalarField(table, numLogT, indLogT, indLogR);
	return true;
}


bool OpalTable::load(const char *opalTableFname) {

	FILE *fp = fopen(opalTableFname, "rb");
	bool rv = fp != NULL;

	rv &= fread(&numLogT, sizeof(numLogT), 1, fp) == 1;
	rv &= fread(&numLogR, sizeof(numLogR), 1, fp) == 1;
	rv &= fread(&minLogT, sizeof(minLogT), 1, fp) == 1;
	rv &= fread(&maxLogT, sizeof(maxLogT), 1, fp) == 1;
	rv &= fread(&minLogR, sizeof(minLogR), 1, fp) == 1;
	rv &= fread(&maxLogR, sizeof(maxLogR), 1, fp) == 1;

	table = new double[numLogT * numLogR];

	rv &= fread(table, numLogT * numLogR * sizeof(table[0]), 1, fp) == 1;
	rv &= fclose(fp) == 0;

	loaded = rv;
	return rv;
}


double OpalTable::opacity(double T, double rho) {

	assert(loaded);
	double rhoCGS = rho * 1e-3; // convert rho to CGS

	// definition of R in logR taken from the OPAL header: 
	// R = density[g/cm**3]/T6**3, T6=1.e-6*T[degrees]

	double T6 = 1e-6 * T;
	double R = rhoCGS / pow(T6, 3); 

	double logR = log10(R);
	double logT = log10(T);
	double logKappa = 0;

	if(interpTable(logT, logR, logKappa)) return pow(10, logKappa) * 1e-1;
	else throw 1;
}

