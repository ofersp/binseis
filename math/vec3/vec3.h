
#ifndef VEC3_H
#define VEC3_H

#include <iostream>

template <class T>
class Vec3 {

public:

	T x, y, z;

	Vec3() {};
	Vec3(T o) { x = o; y = o; z = o; }
	Vec3(const Vec3<T>& o) { x = o.x; y = o.y; z = o.z; }
	Vec3(const T* o) { x = o[0]; y = o[1]; z = o[2]; }
	Vec3(T ox, T oy, T oz) { x = ox; y = oy; z = oz; }
	Vec3<T>& operator=(T o) { x = o; y = o; z = o; return *this; }

	template <class To>
	Vec3<T>& operator=(const Vec3<To>& o) {	x = o.x; y = o.y; z = o.z; return *this; }

	// vector-scallar operations

 	Vec3<T>& operator+=(T s) { x += s; y += s; z += s; return *this; }
 	Vec3<T>& operator-=(T s) { x -= s; y -= s; z -= s; return *this; }
 	Vec3<T>& operator*=(T s) { x *= s; y *= s; z *= s; return *this; }
 	Vec3<T>& operator/=(T s) { x /= s; y /= s; z /= s; return *this; }

 	const Vec3<T> operator+(T s) const { return Vec3<T>(*this) += s; }
	const Vec3<T> operator-(T s) const { return Vec3<T>(*this) -= s; }
 	const Vec3<T> operator*(T s) const { return Vec3<T>(*this) *= s; }
 	const Vec3<T> operator/(T s) const { return Vec3<T>(*this) /= s; }

	// vector-vector component-wise operations

 	Vec3<T>& operator+=(const Vec3<T>& o) { x += o.x; y += o.y; z += o.z; return *this; }
 	Vec3<T>& operator-=(const Vec3<T>& o) { x -= o.x; y -= o.y; z -= o.z; return *this; }
 	Vec3<T>& operator*=(const Vec3<T>& o) { x *= o.x; y *= o.y; z *= o.z; return *this; }
 	Vec3<T>& operator/=(const Vec3<T>& o) { x /= o.x; y /= o.y; z /= o.z; return *this; }

 	const Vec3<T> operator+(const Vec3<T>& o) const { return Vec3<T>(*this) += o; }
 	const Vec3<T> operator-(const Vec3<T>& o) const { return Vec3<T>(*this) -= o; }

	// dot product

 	T operator*(const Vec3<T>& o) const { return x * o.x + y * o.y + z * o.z; } 

	// cross product

 	const Vec3 operator/(const Vec3<T>& o) const { 
		return Vec3<T>(y*o.z-z*o.y, z*o.x-x*o.z, x*o.y-y*o.x);
	}

	// comparison

	bool operator==(const Vec3<T>& o){ return x == o.x && x == o.x && z == o.z; }

	// stream output

	friend std::ostream& operator<<(std::ostream& out, const Vec3<int>& v);
	friend std::ostream& operator<<(std::ostream& out, const Vec3<double>& v);
};

// template specializations

typedef Vec3<double> Vec3d;
typedef Vec3<float> Vec3f;
typedef Vec3<int> Vec3i;

#endif
