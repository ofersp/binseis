#ifndef EOS_TABLES_H
#define EOS_TABLES_H

class EosTables {

private:

	bool loaded;

	int numLogT6, numLogP12;
	double minLogT6, maxLogT6;
	double minLogP12, maxLogP12;

	float *tableRho;
	float *tableDE12dT6v;
	float *tableGamma1;
	float *tableGamma2;
	float *tableChiRho;
	float *tableChiT;

	bool interpLogRho(double logT6, double logP12, double &logRho);
	bool interpDE12dT6v(double logT6, double logP12, double &dE12dT6v);
	bool interpGamma1(double logT6, double logP12, double &Gamma1);
	bool interpGamma2(double logT6, double logP12, double &Gamma2);
	bool interpChiRho(double logT6, double logP12, double &ChiRho);
	bool interpChiT(double logT6, double logP12, double &ChiT);

public:

	EosTables() {	
		loaded = false;
		tableRho = 0;
		tableDE12dT6v = 0;
		tableGamma1 = 0;
		tableGamma2 = 0;
		tableChiRho = 0;
		tableChiT = 0;
	}

	~EosTables() { 
		if(tableRho) delete[] tableRho; 
		if(tableDE12dT6v) delete[] tableDE12dT6v; 
		if(tableGamma1) delete[] tableGamma1;
		if(tableGamma2) delete[] tableGamma2;
		if(tableChiRho) delete[] tableChiRho;
		if(tableChiT) delete[] tableChiT;
	}

	bool load(const char *eosTablesFname);

	double rho(double T, double P);
	double Cv(double T, double P);
	double Gamma1(double T, double P);
	double Gamma2(double T, double P);
	double ChiRho(double T, double P);
	double ChiT(double T, double P);
};

#endif // EOS_TABLES_H

