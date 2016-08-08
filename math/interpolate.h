
#ifndef INTERPOLATE_H
#define INTERPOLATE_H

#include "vec3.h"

double interpScalarArray(const double *s, double x);

double interp2dScalarField(const double *sf, int wx, double x, double y);
float  interp2dScalarFieldFloat(const float *sf, int wx, float x, float y);

double interp3dScalarField(const double *sf, const Vec3i& dim, const Vec3d& p);
Vec3d  interp3dVectorField(const double *vf, const Vec3i& dim, const Vec3d& p);

#endif
