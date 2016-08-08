#ifndef KT_TABLES_H
#define KT_TABLES_H

#include <assert.h>

class KtTables {

private:

	int size;
	double minR, maxR, midR;
	double *tableU;
	double *tableV;
	double *tableZ;
	double *tableW;
	double *tableFp;
	double *tableFt;

	double D, M;
	bool loaded;

public:

	KtTables() {

		loaded = false;

		tableU = 0;
		tableV = 0;
		tableZ = 0;
		tableW = 0;
		tableFp = 0;
		tableFt = 0;
	}
	
	~KtTables() { 
		if(tableU) delete[] tableU;
		if(tableV) delete[] tableV;
		if(tableZ) delete[] tableZ;
		if(tableW) delete[] tableW;
		if(tableFp) delete[] tableFp;
		if(tableFt) delete[] tableFt;
	}

	double u(double r);
	double v(double r);
	double z(double r);
	double w(double r);
	double fp(double r);
	double ft(double r);

	/** @param D distance between the star's core and the L1 in meters. 
		This defines the length scale for the distance parameter r when 
		accessing the tables */

	bool load(const char *ktTablesFname, double D, double M);
};

#endif // KT_TABLES_H

