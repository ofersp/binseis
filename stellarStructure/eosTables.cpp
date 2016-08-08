#include <math.h>
#include <stdio.h>
#include <assert.h>

#include <constants.h>
#include <interpolate.h>
#include "eosTables.h"


bool EosTables::interpLogRho(double logT6, double logP12, double &logRho) {

	double indLogT6 = ((logT6 - minLogT6) / (maxLogT6 - minLogT6)) * (numLogT6 - 1);
	double indLogP12 = ((logP12 - minLogP12) / (maxLogP12 - minLogP12)) * (numLogP12 - 1);

	if(indLogT6 < 0 || indLogT6 > numLogT6 - 1) return false;
	if(indLogP12 < 0 || indLogP12 > numLogP12 - 1) return false;

	logRho = interp2dScalarFieldFloat(tableRho, numLogT6, indLogT6, indLogP12);
	return !isnan(logRho);
}	


bool EosTables::interpDE12dT6v(double logT6, double logP12, double &dE12dT6v) {

	double indLogT6 = ((logT6 - minLogT6) / (maxLogT6 - minLogT6)) * (numLogT6 - 1);
	double indLogP12 = ((logP12 - minLogP12) / (maxLogP12 - minLogP12)) * (numLogP12 - 1);

	if(indLogT6 < 0 || indLogT6 > numLogT6 - 1) return false;
	if(indLogP12 < 0 || indLogP12 > numLogP12 - 1) return false;

	dE12dT6v = interp2dScalarFieldFloat(tableDE12dT6v, numLogT6, indLogT6, indLogP12);
	return !isnan(dE12dT6v);
}	


bool EosTables::interpGamma1(double logT6, double logP12, double &Gamma1) {

	double indLogT6 = ((logT6 - minLogT6) / (maxLogT6 - minLogT6)) * (numLogT6 - 1);
	double indLogP12 = ((logP12 - minLogP12) / (maxLogP12 - minLogP12)) * (numLogP12 - 1);

	if(indLogT6 < 0 || indLogT6 > numLogT6 - 1) return false;
	if(indLogP12 < 0 || indLogP12 > numLogP12 - 1) return false;

	Gamma1 = interp2dScalarFieldFloat(tableGamma1, numLogT6, indLogT6, indLogP12);
	return !isnan(Gamma1);
}	


bool EosTables::interpGamma2(double logT6, double logP12, double &Gamma2) {

	double indLogT6 = ((logT6 - minLogT6) / (maxLogT6 - minLogT6)) * (numLogT6 - 1);
	double indLogP12 = ((logP12 - minLogP12) / (maxLogP12 - minLogP12)) * (numLogP12 - 1);

	if(indLogT6 < 0 || indLogT6 > numLogT6 - 1) return false;
	if(indLogP12 < 0 || indLogP12 > numLogP12 - 1) return false;

	Gamma2 = interp2dScalarFieldFloat(tableGamma2, numLogT6, indLogT6, indLogP12);
	return !isnan(Gamma2);
}


bool EosTables::interpChiRho(double logT6, double logP12, double &ChiRho) {

	double indLogT6 = ((logT6 - minLogT6) / (maxLogT6 - minLogT6)) * (numLogT6 - 1);
	double indLogP12 = ((logP12 - minLogP12) / (maxLogP12 - minLogP12)) * (numLogP12 - 1);

	if(indLogT6 < 0 || indLogT6 > numLogT6 - 1) return false;
	if(indLogP12 < 0 || indLogP12 > numLogP12 - 1) return false;

	ChiRho = interp2dScalarFieldFloat(tableChiRho, numLogT6, indLogT6, indLogP12);
	return !isnan(ChiRho);
}	


bool EosTables::interpChiT(double logT6, double logP12, double &ChiT) {

	double indLogT6 = ((logT6 - minLogT6) / (maxLogT6 - minLogT6)) * (numLogT6 - 1);
	double indLogP12 = ((logP12 - minLogP12) / (maxLogP12 - minLogP12)) * (numLogP12 - 1);

	if(indLogT6 < 0 || indLogT6 > numLogT6 - 1) return false;
	if(indLogP12 < 0 || indLogP12 > numLogP12 - 1) return false;

	ChiT = interp2dScalarFieldFloat(tableChiT, numLogT6, indLogT6, indLogP12);
	return !isnan(ChiT);
}	


bool EosTables::load(const char *eosTablesFname) {

	FILE *fp = fopen(eosTablesFname, "rb");
	bool rv = fp != NULL;

	rv &= fread(&numLogT6, sizeof(numLogT6), 1, fp) == 1;
	rv &= fread(&numLogP12, sizeof(numLogP12), 1, fp) == 1;
	rv &= fread(&minLogT6, sizeof(minLogT6), 1, fp) == 1;
	rv &= fread(&maxLogT6, sizeof(maxLogT6), 1, fp) == 1;
	rv &= fread(&minLogP12, sizeof(minLogP12), 1, fp) == 1;
	rv &= fread(&maxLogP12, sizeof(maxLogP12), 1, fp) == 1;

	tableRho = new float[numLogT6 * numLogP12];
	tableDE12dT6v = new float[numLogT6 * numLogP12];
	tableGamma1 = new float[numLogT6 * numLogP12];
	tableGamma2 = new float[numLogT6 * numLogP12];
	tableChiRho = new float[numLogT6 * numLogP12];
	tableChiT = new float[numLogT6 * numLogP12];

	rv &= fread(tableRho, numLogT6 * numLogP12 * sizeof(tableRho[0]), 1, fp) == 1;
	rv &= fread(tableDE12dT6v, numLogT6 * numLogP12 * sizeof(tableDE12dT6v[0]), 1, fp) == 1;
	rv &= fread(tableGamma1, numLogT6 * numLogP12 * sizeof(tableGamma1[0]), 1, fp) == 1;
	rv &= fread(tableGamma2, numLogT6 * numLogP12 * sizeof(tableGamma2[0]), 1, fp) == 1;
	rv &= fread(tableChiRho, numLogT6 * numLogP12 * sizeof(tableChiRho[0]), 1, fp) == 1;
	rv &= fread(tableChiT, numLogT6 * numLogP12 * sizeof(tableChiT[0]), 1, fp) == 1;

	rv &= fclose(fp) == 0;

	loaded = rv;
	return rv;
}


double EosTables::rho(double T, double P) {

	assert(loaded);
	if (T<=0 || P<=0) throw 1;

	double logT6 = log10(T * 1e-6);
	double logP12 = log10(P * 1e-11);

	double logRhoCGS;
	if(interpLogRho(logT6, logP12, logRhoCGS)) return pow(10.0, logRhoCGS) * 1e3;
	else throw 1;
}


double EosTables::Cv(double T, double P) {

	assert(loaded);
	assert(T>0 && P>0);
	double logT6 = log10(T * 1e-6);
	double logP12 = log10(P * 1e-11);

	double dE12dT6v, Cv;
	if(interpDE12dT6v(logT6, logP12, dE12dT6v)) {
		Cv = dE12dT6v * 1e12 * 1e-6 * 1e-4; 
		return Cv;
	}
	else throw 1;
}


double EosTables::Gamma1(double T, double P) {

	assert(loaded);
	assert(T>0 && P>0);
	double logT6 = log10(T * 1e-6);
	double logP12 = log10(P * 1e-11);

	double Gamma1;
	if(interpGamma1(logT6, logP12, Gamma1))	return Gamma1;
	else throw 1;
}


double EosTables::Gamma2(double T, double P) {

	assert(loaded);
	assert(T>0 && P>0);
	double logT6 = log10(T * 1e-6);
	double logP12 = log10(P * 1e-11);

	double Gamma2;
	if(interpGamma2(logT6, logP12, Gamma2))	return Gamma2;
	else throw 1;
}


double EosTables::ChiRho(double T, double P) {

	assert(loaded);
	assert(T>0 && P>0);
	double logT6 = log10(T * 1e-6);
	double logP12 = log10(P * 1e-11);

	double ChiRho;
	if(interpChiRho(logT6, logP12, ChiRho)) return ChiRho;
	else throw 1;
}


double EosTables::ChiT(double T, double P) {

	assert(loaded);
	assert(T>0 && P>0);
	double logT6 = log10(T * 1e-6);
	double logP12 = log10(P * 1e-11);

	double ChiT;
	if(interpChiT(logT6, logP12, ChiT))	return ChiT;
	else throw 1;
}

