#ifndef UTILS_H
#define UTILS_H

#include "vec3.h"


template <class T>
inline void buildCoordSys(const Vec3<T>& a, Vec3<T>& xHat, Vec3<T>& yHat, Vec3<T>& zHat) {

	const double epsilon = 1e-3;

	zHat = a / sqrt(a * a); 
	yHat = Vec3<T>(0,1,0) - zHat * zHat.y;
	double yHatLength = sqrt(yHat * yHat);

	// possibly deal with the singular case where zHat is already
	// in the (1,0,0) direction

	if(yHatLength < epsilon) { 

		yHat = Vec3<T>(1,0,0) - zHat * zHat.x;
		yHatLength = sqrt(yHat * yHat);
	}

	yHat /= yHatLength;
	xHat = yHat / zHat;
}


template <class T>
inline Vec3<T> toCoordSys(const Vec3<T>& a, const Vec3<T>& origin, 
	const Vec3<T>& xHat, const Vec3<T>& yHat, const Vec3<T>& zHat) {

	return origin + xHat * a.x + yHat * a.y + zHat * a.z;
}


template <class T>
inline Vec3<T> fromCoordSys(const Vec3<T>& a, const Vec3<T>& origin, 
	const Vec3<T>& xHat, const Vec3<T>& yHat, const Vec3<T>& zHat) {

	Vec3<T> ta, b = a - origin;
	ta.x = b * xHat;
	ta.y = b * yHat;
	ta.z = b * zHat;	
	return ta;
}

#endif
