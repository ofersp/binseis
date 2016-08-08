
#include <stdio.h>
#include <stdlib.h>
#include "ray.h"


bool Ray::readInitConds(FILE *fp) {

	bool rv = true;

	int lastMaxSamples = maxSamples;

	rv &= fread(&r0, sizeof(r0), 1, fp) == 1;
	rv &= fread(&k0, sizeof(k0), 1, fp) == 1;
	rv &= fread(&branch, sizeof(branch), 1, fp) == 1;
	rv &= fread(&omega, sizeof(omega), 1, fp) == 1;
	rv &= fread(&timeStep, sizeof(timeStep), 1, fp) == 1;
	rv &= fread(&maxSteps, sizeof(maxSteps), 1, fp) == 1;
	rv &= fread(&sampleEach, sizeof(sampleEach), 1, fp) == 1;
	rv &= fread(&maxSamples, sizeof(maxSamples), 1, fp) == 1;

	simSamples = 0;
	surfaceSamples = 0;

	if(!path || maxSamples != lastMaxSamples) {
		if(path) free(path);
		path = (PathSample*)calloc(maxSamples, sizeof(PathSample));
		rv &= path != NULL;
	}
	
	return rv;
}


/** 
	@param writeSurface if true only the atSurface samples are written, with 
		the last simulated sample appended (possibly repeated). Otherwise all simulated 
		samples are written without repeating the last sample */

bool Ray::writePath(FILE *fp, bool writeSurface) {

	bool rv = true;

	rv &= fwrite(&simSamples, sizeof(simSamples), 1, fp) == 1;
	rv &= fwrite(&surfaceSamples, sizeof(surfaceSamples), 1, fp) == 1;
	rv &= fwrite(&stopCond, sizeof(stopCond), 1, fp) == 1;

	if(simSamples == 0) // path is empty
		return rv;

	if(!writeSurface) // write all the simulated samples

		rv &= fwrite(path, sizeof(PathSample), simSamples, fp) == simSamples;

	else { // write only the atSurface samples appending the last sample

		for(int i=0; i<simSamples && rv; i++) {
			if(path[i].atSurface > 0.0)
				rv &= fwrite(&path[i], sizeof(path[i]), 1, fp) == 1;
		}
		rv &= fwrite(&path[simSamples-1], sizeof(path[0]), 1, fp) == 1;
	}
	return rv;
}

