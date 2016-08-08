
#include "mat3x3.h"


std::ostream& operator<<(std::ostream& out, const Mat3x3<int>& m) { 
	out << "[" << m.xx << ", " << m.xy << ", " << m.xz << "]" << std::endl;
	out << "[" << m.yx << ", " << m.yy << ", " << m.yz << "]" << std::endl;
	out << "[" << m.zx << ", " << m.zy << ", " << m.zz << "]";
	return out;
}

std::ostream& operator<<(std::ostream& out, const Mat3x3<double>& m) { 
	out << "[" << m.xx << ", " << m.xy << ", " << m.xz << "]" << std::endl;
	out << "[" << m.yx << ", " << m.yy << ", " << m.yz << "]" << std::endl;
	out << "[" << m.zx << ", " << m.zy << ", " << m.zz << "]";
	return out;
}

