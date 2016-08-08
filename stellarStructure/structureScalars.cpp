
#include <stdio.h>

#include "structureScalars.h"


void StructureScalars::print() {

	double *fieldPtr = (double*)this;
	for(int i=0; i<numFields; ++i)	
		printf("%0.9e ", *(fieldPtr++));
	printf("\n");
}		
