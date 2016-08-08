#ifndef STAR_BUILDER_H
#define STAR_BUILDER_H

#include <math.h>

#include <constants.h>
#include "eosTables.h"
#include "opalTable.h"
#include "ktTables.h"
#include "structureScalars.h"

class StarBuilder {

public:

	StarBuilder(
		int numShells, double M, 
		double Rs, double Ls, double Tc, double Pc, 
		double alphaMLT, double D, double X, double Z, 
		const char *eosTablesFname, const char *opalTableFname, 
		const char *ktTablesFname);

	~StarBuilder() { delete[] ssc; }

	int integrate(int shellStart, int shellStop, 
		double R0, double T0, double L0, double P0);

	void build();
	void fit();
	void print();
	bool buildAndPrintPhotosphere();

private:

	StructureScalars *ssc;

	int numShells, fittingShell;
	int numPhotosphereShells;

	double X, Y, Z;
	double M, Rs, Ls, Tc, Pc;
	double alphaMLT, D;

	EosTables eosTables;
	OpalTable opalTable;
	KtTables ktTables;

	void initMassShells();
	void setFittingShell(double Rs, double Tc, double Ls, double Pc);
	bool sampleStructure(double M, double R, double T, double L, 
		double P, StructureScalars &s, bool inPhotosphere);
	int integrateInwards(double Rs, double Ls, StructureScalars &rssc, 
		bool findOverlap);
	int integrateOutwards(double Tc, double Pc, StructureScalars &rssc, 
		bool findOverlap);
};

#endif // STAR_BUILDER_H

