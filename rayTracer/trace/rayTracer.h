
#ifndef RAY_TRACER_H
#define RAY_TRACER_H

#include <stdlib.h>
#include <vec3.h>
#include "ray.h"
#include "binarySystem.h"


class RayTracer {

public:

	void init(const BinarySystem *bSys);
	void trace(Ray& ray) const;

private:

	double traceStepRK4(Vec3d& r, Vec3d& k, double branch, double timeStep) const;

	double samplePsiExternal(const Vec3d &r) const;
	void smoothingWeigths(double R1, double &innerW, double &outerW) const;

	bool sampleKT(double psiTilde, double &rTilde) const;
	bool sampleSST(double *F, double r, double &f) const;
	bool sampleG(const Vec3d &r, double &g, Vec3d &gHat) const;
	bool sampleStructure(const Vec3d &r, double &P, double &rho, double &Gamma1) const;

  double omega(const Vec3d& r, const Vec3d& k, double branch) const;
  double omegaGough(const Vec3d& k, double omegaASqr, double aSqr) const;
  double omegaIsothermal(const Vec3d& k, double branch,
		const Vec3d& gHat, double omegaASqr, double aSqr, double Gamma1) const;

  double atSurface(const Vec3d &r, const Vec3d &k, double omega) const;    

private:

	double epsR, epsK;
	Vec3d drx, dry, drz;
	Vec3d dkx, dky, dkz;
	const BinarySystem *bSys;
};

#endif
