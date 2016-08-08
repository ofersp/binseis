
#include "vec3.h"


std::ostream& operator<<(std::ostream& out, const Vec3<int>& v) { 
	out << "[" << v.x << ", " << v.y << ", " << v.z << "]";
	return out;
}


std::ostream& operator<<(std::ostream& out, const Vec3<double>& v) { 
	out << "[" << v.x << ", " << v.y << ", " << v.z << "]";
	return out;
}

