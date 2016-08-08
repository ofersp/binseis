
#include <math.h>
#include "rayTracer.h"


double RayTracer::traceStepRK4(Vec3d& r, Vec3d& k, 
	double branch, double timeStep) const { 

	double currOmega;
	Vec3d drdt1, drdt2, drdt3, drdt4;
	Vec3d dkdt1, dkdt2, dkdt3, dkdt4;

	Vec3d r0 = r;
	Vec3d k0 = k;

	currOmega = omega(r, k, branch);
	dkdt1.x = -(omega(r + drx, k, branch) - currOmega);
	dkdt1.y = -(omega(r + dry, k, branch) - currOmega);
	dkdt1.z = -(omega(r + drz, k, branch) - currOmega); dkdt1 /= epsR;
	drdt1.x =  (omega(r, k + dkx, branch) - currOmega);
	drdt1.y =  (omega(r, k + dky, branch) - currOmega);
	drdt1.z =  (omega(r, k + dkz, branch) - currOmega); drdt1 /= epsK;

	r = r0 + drdt1 * (timeStep / 2);
	k = k0 + dkdt1 * (timeStep / 2);
	currOmega = omega(r, k, branch);
	dkdt2.x = -(omega(r + drx, k, branch) - currOmega);
	dkdt2.y = -(omega(r + dry, k, branch) - currOmega);
	dkdt2.z = -(omega(r + drz, k, branch) - currOmega); dkdt2 /= epsR;
	drdt2.x =  (omega(r, k + dkx, branch) - currOmega);
	drdt2.y =  (omega(r, k + dky, branch) - currOmega);
	drdt2.z =  (omega(r, k + dkz, branch) - currOmega); drdt2 /= epsK;

	r = r0 + drdt2 * (timeStep / 2);
	k = k0 + dkdt2 * (timeStep / 2);
	currOmega = omega(r, k, branch);
	dkdt3.x = -(omega(r + drx, k, branch) - currOmega);
	dkdt3.y = -(omega(r + dry, k, branch) - currOmega);
	dkdt3.z = -(omega(r + drz, k, branch) - currOmega); dkdt3 /= epsR;
	drdt3.x =  (omega(r, k + dkx, branch) - currOmega);
	drdt3.y =  (omega(r, k + dky, branch) - currOmega);
	drdt3.z =  (omega(r, k + dkz, branch) - currOmega); drdt3 /= epsK;

	r = r0 + drdt3 * timeStep;
	k = k0 + dkdt3 * timeStep;
	currOmega = omega(r, k, branch);
	dkdt4.x = -(omega(r + drx, k, branch) - currOmega);
	dkdt4.y = -(omega(r + dry, k, branch) - currOmega);
	dkdt4.z = -(omega(r + drz, k, branch) - currOmega); dkdt4 /= epsR;
	drdt4.x =  (omega(r, k + dkx, branch) - currOmega);
	drdt4.y =  (omega(r, k + dky, branch) - currOmega);
	drdt4.z =  (omega(r, k + dkz, branch) - currOmega); drdt4 /= epsK;

	r = r0 + (drdt1 + drdt2*2.0 + drdt3*2.0 + drdt4) * (timeStep / 6.0); 
	k = k0 + (dkdt1 + dkdt2*2.0 + dkdt3*2.0 + dkdt4) * (timeStep / 6.0);

	return omega(r, k, branch); 
}

