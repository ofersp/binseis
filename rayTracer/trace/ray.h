
#ifndef RAY_H
#define RAY_H

#include <vec3.h>


class Ray {

public:

	Ray() { path = 0; maxSamples = 0; }
	~Ray() { if(path) free(path); }

	bool readInitConds(FILE *fp);
	bool writePath(FILE *fp, bool writeSurface);

public:

	typedef struct {
		Vec3f r, k;
		float omega;
    float atSurface;
	} PathSample;

public:

	/** 
		encounteredNeg results when a ray propagated beyond the photosphere, 
		encounteredNaN is an error condition,
		outOfBounds results when a ray propagated beyond the L1,
		completedRun when simulation stopped mid-flight */
 
	enum StopCond {encounteredNeg, encounteredNaN, outOfBounds, completedRun};

	Vec3d r0, k0;
	double branch, omega, timeStep;
	int maxSteps, sampleEach, maxSamples;

	int simSamples;
	int surfaceSamples;
	StopCond stopCond;
	PathSample* path;
};

#endif
