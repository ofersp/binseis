#include <math.h>
#include <stdio.h>
#include <assert.h>

#include <interpolate.h>
#include <constants.h>
#include "ktTables.h"


double KtTables::u(double r) {

	assert(loaded);	
	double index = (((r / D) - minR) / maxR) * size;
	if(index < 0) return 1.0;
	if(index >= size) tableU[size-1];
	double u = interpScalarArray(tableU, index);
	return u;
}


double KtTables::v(double r) {

	assert(loaded);	
	double index = (((r / D) - minR) / maxR) * size;
	if(index < 0) return 1.0;
	if(index >= size) tableV[size-1];
	double v = interpScalarArray(tableV, index);
	return v;
}


double KtTables::z(double r) {

	assert(loaded);	
	double index = (((r / D) - minR) / maxR) * size;
	if(index < 0) return 1.0;
	if(index >= size) tableZ[size-1];
	double z = interpScalarArray(tableZ, index);
	return z;
}


double KtTables::w(double r) {

	assert(loaded);	
	double index = (((r / D) - minR) / maxR) * size;
	if(index < 0) return 1.0;
	if(index >= size) tableW[size-1];
	double w = interpScalarArray(tableW, index);
	return w;
}


double KtTables::fp(double r) {

	assert(loaded);	
	double index = (((r / D) - minR) / maxR) * size;
	if(index < 0) return 1.0;
	if(index >= size) tableFp[size-1];
	double fp = interpScalarArray(tableFp, index);
	return fp;
}


double KtTables::ft(double r) {
	
	assert(loaded);	
	double index = (((r / D) - minR) / maxR) * size;
	if(index < 0) return 1.0;
	if(index >= size) tableFt[size-1];
	double ft = interpScalarArray(tableFt, index);
	return ft;
}


bool KtTables::load(const char *ktTablesFname, double D, double M) {

	this->D = D;
	this->M = M;

	FILE *fp = fopen(ktTablesFname, "rb");
	bool rv = fp != NULL;

  rv &= fread(&size, sizeof(size), 1, fp) == 1;
	rv &= fread(&minR, sizeof(minR), 1, fp) == 1;
	rv &= fread(&maxR, sizeof(maxR), 1, fp) == 1;
	rv &= fread(&midR, sizeof(midR), 1, fp) == 1;

	tableU = new double[size];
	tableV = new double[size];
	tableZ = new double[size];
	tableW = new double[size];
	tableFp = new double[size];
	tableFt = new double[size];

	rv &= fread(tableU, size * sizeof(tableU[0]), 1, fp) == 1;
	rv &= fread(tableV, size * sizeof(tableV[0]), 1, fp) == 1;
	rv &= fread(tableZ, size * sizeof(tableZ[0]), 1, fp) == 1;
	rv &= fread(tableW, size * sizeof(tableW[0]), 1, fp) == 1;
	rv &= fread(tableFp, size * sizeof(tableFp[0]), 1, fp) == 1;
	rv &= fread(tableFt, size * sizeof(tableFt[0]), 1, fp) == 1;

	rv &= fclose(fp) == 0;

	loaded = rv;
	return rv;
}

