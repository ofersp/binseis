#ifndef OPAL_TABLE_H
#define OPAL_TABLE_H

class OpalTable {

private:

	int numLogT, numLogR;
	double minLogT, maxLogT;
	double minLogR, maxLogR;
	double *table;
	bool loaded;

	bool interpTable(double logT, double logR, double &logKappa);

public:

	OpalTable() {	table = 0; loaded = false; }	
	~OpalTable() { if(table) delete[] table; }

	bool load(const char *opalTableFname);
	double opacity(double T, double rho);
};

#endif // OPAL_TABLE_H

