
#include <iostream>
#include "../../misc/unitTest.h"
#include "vec3.h"

using namespace std;


class testVec3 : public unitTest { 
public:
	testVec3() {} 
	bool run() {

		Vec3d v1; v1 = 1;
		cout << "Vec3d v1; v1 = 1;" << endl;
		logSubTestStatus(v1 == Vec3d(1,1,1), true);
		cout << "v1: " << v1 << endl;

		Vec3d v2(2);
		cout << "Vec3d v2(2);" << endl;
		logSubTestStatus(v2 == Vec3d(2,2,2), true);
		cout << "v2: " << v2 << endl;

		double arr[] = {1.3, 2.3, 3.3};
		cout << "double arr[] = {1.3, 2.3, 3.3};" << endl;
		Vec3d v3(arr);	
		cout << "Vec3d v3(arr);" << endl;
		logSubTestStatus(v3 == Vec3d(1.3,2.3,3.3), true);	
		cout << "v3: " << v3 << endl;

		Vec3d v4(1, 2, 3);
		cout << "Vec3d v4(1, 2, 3);" << endl;
		logSubTestStatus(v4 == Vec3d(1,2,3), true);	
		cout << "v4: " << v4 << endl;

		Vec3d v5(v4);
		cout << "Vec3d v5(v4);" << endl;
		logSubTestStatus(v5 == Vec3d(1,2,3), true);	
		cout << "v5: " << v5 << endl;

		v4 += 0.5;
		cout << "v4 += 0.5;" << endl;
		logSubTestStatus(v4 == Vec3d(1.5,2.5,3.5), true);	
		cout << "v4: " << v4 << endl;

		v4 -= 0.5;
		cout << "v4 -= 0.5;" << endl;
		logSubTestStatus(v4 == Vec3d(1,2,3), true);	
		cout << "v4: " << v4 << endl;

		v4 *= 0.5;
		cout << "v4 *= 0.5;" << endl;
		logSubTestStatus(v4 == Vec3d(0.5, 1, 1.5), true);	
		cout << "v4: " << v4 << endl;

		v4 /= 0.5;
		cout << "v4 /= 0.5;" << endl;
		logSubTestStatus(v4 == Vec3d(1,2,3), true);	
		cout << "v4: " << v4 << endl;

		v4 = v4 += 0.5;
		cout << "v4 = v4 += 0.5;" << endl;
		logSubTestStatus(v4 == Vec3d(1.5,2.5,3.5), true);	
		cout << "v4: " << v4 << endl;

		Vec3d v6 = ((v5 + 1) * 2 - 3) / 4;
		cout << "Vec3d v6 = ((v5 + 1) * 2 - 3) / 4;" << endl;
		logSubTestStatus(v6 == Vec3d(0.25,0.75,1.25), true);	
		cout << "v6: " << v6 << endl;

		v3 -= v4;
		cout << "v3 -= v4;" << endl;
		logSubTestStatus(v3 == Vec3d(1.3-1.5,2.3-2.5,3.3-3.5), true);	
		cout << "v3: " << v3 << endl;

		v3 += v4; v3 /= v1 * 2;
		cout << "v3 += v4; v3 /= v1 * 2;" << endl;
		logSubTestStatus(v3 == Vec3d(0.65,1.15,1.65), true);	
		cout << "v3: " << v3 << endl;

		v3 = v5; v3 *= v5;
		cout << "v3 = v5; v3 *= v5;" << endl;
		logSubTestStatus(v3 == Vec3d(1,4,9), true);		
		cout << "v3: " << v3 << endl;

		v3 /= v3;
		cout << "v3 /= v3;" << endl;
		logSubTestStatus(v3 == Vec3d(1,1,1), true);	
		cout << "v3: " << v3 << endl;

		v3 = (v3 + v4) - v6;
		cout << "v3 = (v3 + v4) - v6;" << endl;
		logSubTestStatus(v3 == Vec3d(2.25,2.75,3.25), true);	
		cout << "v3: " << v3 << endl;

		double s1 = v5 * v5;
		cout << "double s1 = v5 * v5;" << endl;
		logSubTestStatus(s1 == 14, true);	
		cout << "s1: " << s1 << endl;

		v6 = v5 / v1;
		cout << "v6 = v5 / v1;" << endl;
		logSubTestStatus(v6 == Vec3d(-1,2,-1), true);	
		cout << "v6: " << v6 << endl;

		v6 = v1 / v5;
		cout << "v6 = v1 / v5;" << endl;
		logSubTestStatus(v6 == Vec3d(1,-2,1), true);	
		cout << "v6: " << v6 << endl;
	}
};


int main(int argc, char **argv) {

	testVec3 t;

	t.begin();
	t.run();
	t.finalize();
}
