
#include <stdio.h>
#include <stdlib.h>
#include <cstring>

#include "binarySystem.h"
#include "ray.h"
#include "rayTracer.h"


int main(int argc, char **argv) {

	const char* fnameBinarySystem = "io/binarySystem.bin";
	const char* fnameRayInitCondQueue = "io/rayInitCondQueue.bin";
	const char* fnameRayPathQueue = "io/rayPathQueue.bin";

	// read command line

	bool writeSurface = false;
	if(argc == 2)	
		writeSurface = strcmp(argv[1], "writeSurface") == 0;

	// read binary system
	
	BinarySystem bSys;

	if(!bSys.read(fnameBinarySystem)) {
		printf("Failed to open %s\n", fnameBinarySystem);
		exit(1);
	}	

	// initialize the raytracer with the binary system

	RayTracer rt;
	rt.init(&bSys);

	printf("\nLoading of %s complete.\n", fnameBinarySystem);
	printf("Tracing rays given in %s\n", fnameRayInitCondQueue);
	printf("and saving resulting paths into %s.\n", fnameRayPathQueue);

	// open fnameRayInitCondQueue for input and fnameRayPathQueue for output	

	FILE *fpInp = fopen(fnameRayInitCondQueue, "rb");
	fseek(fpInp, 0, SEEK_END);
	int inpLength = ftell(fpInp);
	rewind(fpInp);

	if(!fpInp) {
		printf("Failed to open %s\n", fnameRayInitCondQueue);
		exit(3);
	}

	FILE *fpOut = fopen(fnameRayPathQueue, "wb");

	if(!fpOut) {
		printf("Failed to open %s\n", fnameRayPathQueue);
		exit(4);
	}

	// read, trace and write each ray

	Ray ray;
	int rayCounter = 0;	

	while(ftell(fpInp) < inpLength) {

    printf("\rProcessing ray #%d", rayCounter+1);
    fflush(stdout);
    
		if(!ray.readInitConds(fpInp)) {
			printf("Failed to read from %s\n", fnameRayInitCondQueue);
			exit(5);
		}

		rt.trace(ray);

		if(!ray.writePath(fpOut, writeSurface)) {
			printf("Failed to write to %s\n", fnameRayPathQueue);
			exit(6);
		}
		rayCounter++;
	}

	printf("\nDone\n\n");
	return 0;
}

