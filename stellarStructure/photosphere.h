#ifndef PHOTOSPHERE_H
#define PHOTOSPHERE_H

#include "eosTables.h"
#include "opalTable.h"
#include "ktTables.h"
#include "starBuilder.h"

/**
 * Note that the subscripts in Photosphere and StarBuilder are different.
 * Here we denote base-photospheric values (where tau = 2/3) of a quantity 
 * X as Xe, with values at the actual surface (where tau = 0) denoted as 
 * Xs. In StarBuilder quantities with the subscript "s" represent their 
 * value at the base of the photosphere */

namespace Photosphere {
	
	double temperature(double tau, double Te);
	double basalPressure(double Me, double Re, double Te, double D,
		EosTables &eosTables, OpalTable &opalTable, KtTables &ktTables, 
		int numShells, StructureScalars *ssc);
}

#endif // PHOTOSPHERE_H
