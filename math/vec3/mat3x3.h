
#ifndef MAT3x3_H
#define MAT3x3_H

#include <iostream>
#include "vec3.h"


template <class T>
class Mat3x3 {

public:

	T xx, xy, xz;
	T yx, yy, yz;
	T zx, zy, zz;

	Mat3x3() {};

	Mat3x3<T>& operator=(const Mat3x3<T>& o) { 
		xx = o.xx; xy = o.xy; xz = o.xz;
		yx = o.yx; yy = o.yy; yz = o.yz;
		zx = o.zx; zy = o.zy; zz = o.zz;
		return *this; 
	}

	Mat3x3(T oxx, T oxy, T oxz,
		T oyx, T oyy, T oyz,
		T ozx, T ozy, T ozz) {

		xx = oxx; xy = oxy; xz = oxz;
		yx = oyx; yy = oyy; yz = oyz;
		zx = ozx; zy = ozy; zz = ozz;
	}

	// unary operations

	/** multiply this matrix from the right by given matrix */

 	Mat3x3<T>& operator*=(const Mat3x3<T>& o) { 
		
		Mat3x3<T> n;

		n.xx = xx * o.xx + xy * o.yx + xz * o.zx;
		n.yx = yx * o.xx + yy * o.yx + yz * o.zx;
		n.zx = zx * o.xx + zy * o.yx + zz * o.zx;

		n.xy = xx * o.xy + xy * o.yy + xz * o.zy;
		n.yy = yx * o.xy + yy * o.yy + yz * o.zy;
		n.zy = zx * o.xy + zy * o.yy + zz * o.zy;

		n.xz = xx * o.xz + xy * o.yz + xz * o.zz;
		n.yz = yx * o.xz + yy * o.yz + yz * o.zz;
		n.zz = zx * o.xz + zy * o.yz + zz * o.zz;

		*this = n;
		return *this; 
	}

	// binary operations

	/** multiply given vector from the left by this matrix */

 	const Vec3<T> operator*(const Vec3<T>& v) const { 
		
		Vec3<T> n;

		n.x = xx * v.x + xy * v.y + xz * v.z;
		n.y = yx * v.x + yy * v.y + yz * v.z;
		n.z = zx * v.x + zy * v.y + zz * v.z;

		return n; 
	}

	/** multiply this matrix from the right by given matrix */
	
 	const Mat3x3<T> operator*(const Mat3x3<T>& o) const { 
		return Mat3x3<T>(*this) *= o; 
	}

	const T det() const {
		
		return 
			xx * (yy * zz - yz * zy) +
			xy * (yz * zx - yx * zz) +
			xz * (yx * zy - yy * zx);
	}

	const Mat3x3<T> inv() const {
		Mat3x3<T> mi;

		double invDet = 1/det();

		mi.xx = (yy * zz - yz * zy) * invDet;
		mi.xy = (xz * zy - xy * zz) * invDet;
		mi.xz = (xy * yz - xz * yy) * invDet;

		mi.yx = (yz * zx - yx * zz) * invDet;
		mi.yy = (xx * zz - xz * zx) * invDet;
		mi.yz = (xz * yx - xx * yz) * invDet;

		mi.zx = (yx * zy - yy * zx) * invDet;
		mi.zy = (xy * zx - xx * zy) * invDet;
		mi.zz = (xx * yy - xy * yx) * invDet;

		return mi;
	}

	// stream output

	friend std::ostream& operator<<(std::ostream& out, const Mat3x3<int>& m);
	friend std::ostream& operator<<(std::ostream& out, const Mat3x3<double>& m);
};

// template specializations

typedef Mat3x3<double> Mat3x3d;
typedef Mat3x3<float> Mat3x3f;
typedef Mat3x3<int> Mat3x3i;

#endif
