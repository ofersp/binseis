
#include <stdlib.h>
#include <stdio.h>

#include "starBuilder.h"


int main(int argc, char **argv) {

	if(argc != 10) {

		printf("\nWrong number of arguments. ");
		printf("Expected syntax: \nbstar ");
		printf("[numShells] [M] [Rs] [Tc] [Ls] [Pc] [alphaMLT] [D] [f/b]\n\n");
		return 1;
	}

	int i = 1;
	const int numShells = atoi(argv[i++]);

	if(numShells % 4 != 0) { // see definition of numPhotosphereShells in StarBuilder::StarBuilder()
		printf("\nnumShells must be a multiple of 4");
		return 1;
	}

	const double M = atof(argv[i++]) * M_Sun;
	const double Rs = atof(argv[i++]) * R_Sun;
	const double Tc = atof(argv[i++]);
	const double Ls = atof(argv[i++]) * L_Sun;;
	const double Pc = atof(argv[i++]);
	const double alphaMLT = atof(argv[i++]);
	const double D = atof(argv[i++]) * R_Sun;
	const bool fit = argv[i][0] == 'f';

	const double X = 0.7;
	const double Z = 0.02;

	const char *eosTablesFname = "../data/tables/eosX70Z02.bin";
	const char *opalTableFname = "../data/tables/opalX70Z02.bin";
	const char *ktTablesFname = "../data/tables/kt.bin";

	StarBuilder sb(numShells, M, 
		Rs, Ls, Tc, Pc,	alphaMLT, D, X, Z, 
		eosTablesFname, opalTableFname, ktTablesFname);

	if(fit)
		sb.fit();

	else { 
		sb.build(); 
		sb.print(); 
		sb.buildAndPrintPhotosphere();
	}
	return 0;
}
