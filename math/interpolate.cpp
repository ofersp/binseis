
#include "interpolate.h"


double interpScalarArray(const double *s, double x) {

	int ix = (int)x;
	double dx = x - ix;
	return s[ix] * (1-dx) + s[ix+1] * dx;
}


double interp2dScalarField(const double *sf, int wx, double x, double y) {

	double rv;

	int ix = (int)x;
	int iy = (int)y;

	double dx = x - ix;
	double dy = y - iy;

	double x0y0 = sf[ix+0 + (iy+0) * wx];
	double x0y1 = sf[ix+0 + (iy+1) * wx];
	double x1y0 = sf[ix+1 + (iy+0) * wx];
	double x1y1 = sf[ix+1 + (iy+1) * wx];

	rv  = (x0y0 * (1 - dx) + x1y0 * dx) * (1 - dy);
	rv += (x0y1 * (1 - dx) + x1y1 * dx) * dy;

	return rv;
}


double interp3dScalarField(const double *sf, const Vec3i& dim, const Vec3d& p) {

	double rv;

	int wy = dim.x;
	int wz = dim.x * dim.y;

	int pix = (int)p.x;
	int piy = (int)p.y;
	int piz = (int)p.z;

	double dx = p.x - pix;
	double dy = p.y - piy;
	double dz = p.z - piz;

	double x0y0z0 = sf[(pix+0) + (piy+0) * wy + (piz+0) * wz];
	double x0y0z1 = sf[(pix+0) + (piy+0) * wy + (piz+1) * wz];
	double x0y1z0 = sf[(pix+0) + (piy+1) * wy + (piz+0) * wz];
	double x0y1z1 = sf[(pix+0) + (piy+1) * wy + (piz+1) * wz];
	double x1y0z0 = sf[(pix+1) + (piy+0) * wy + (piz+0) * wz];
	double x1y0z1 = sf[(pix+1) + (piy+0) * wy + (piz+1) * wz];
	double x1y1z0 = sf[(pix+1) + (piy+1) * wy + (piz+0) * wz];
	double x1y1z1 = sf[(pix+1) + (piy+1) * wy + (piz+1) * wz];

	rv =  ((x0y0z0*(1-dz)+x0y0z1*dz)*(1-dy)+(x0y1z0*(1-dz)+x0y1z1*dz)*dy)*(1-dx);
	rv +=	((x1y0z0*(1-dz)+x1y0z1*dz)*(1-dy)+(x1y1z0*(1-dz)+x1y1z1*dz)*dy)*(  dx);

	return rv;
}


Vec3d interp3dVectorField(const double *vf, const Vec3i& dim, const Vec3d& p) {

	Vec3d v;

	int dimSize = dim.x * dim.y * dim.z;

	v.x = interp3dScalarField(vf + dimSize*0, dim, p);
	v.y = interp3dScalarField(vf + dimSize*1, dim, p);
	v.z = interp3dScalarField(vf + dimSize*2, dim, p);

	return v;
}


float interp2dScalarFieldFloat(const float *sf, int wx, float x, float y) {

	float rv;

	int ix = (int)x;
	int iy = (int)y;

	float dx = x - ix;
	float dy = y - iy;

	float x0y0 = sf[ix+0 + (iy+0) * wx];
	float x0y1 = sf[ix+0 + (iy+1) * wx];
	float x1y0 = sf[ix+1 + (iy+0) * wx];
	float x1y1 = sf[ix+1 + (iy+1) * wx];

	rv  = (x0y0 * (1 - dx) + x1y0 * dx) * (1 - dy);
	rv += (x0y1 * (1 - dx) + x1y1 * dx) * dy;

	return rv;
}
