
#ifndef UNIT_TEST_H
#define UNIT_TEST_H

#include <string>
#include "colorPuts.h"

using namespace std;


class unitTest {

protected:
	int _numSubTestsPerfomed;
	int _numSubTestsPassed;

	void logSubTestStatus(bool status, bool printStatus) {

		_numSubTestsPerfomed++;
		_numSubTestsPassed += (int)status;
		if(printStatus) {
			if(status) ColorPuts::putsnobg((char*)"passed ", true, ColorPuts::GREEN);
			else ColorPuts::putsnobg((char*)"failed ", true, ColorPuts::RED);
		}
	}

public:
	unitTest() {
		_numSubTestsPerfomed = 0;
		_numSubTestsPassed = 0;
	}

	int getSubTestsPerformed() { return _numSubTestsPerfomed; }
	int getSubTestsPassed() { return _numSubTestsPassed; }
	int getSubTestsFailed() { return _numSubTestsPerfomed - _numSubTestsPassed; }
	
	virtual bool run() = 0;

	void begin() { 
		ColorPuts::putsnobg((char*)"unitTest started", true, ColorPuts::YELLOW);
		cout << endl << endl;
	}

	void finalize() {

		cout << endl;
		ColorPuts::putsnobg((char*)"sub-tests performed: ", true, ColorPuts::YELLOW);
		cout << getSubTestsPerformed() << endl;
		ColorPuts::putsnobg((char*)"sub-tests passed: ", true, ColorPuts::YELLOW);
		cout << getSubTestsPassed() << endl;		
		ColorPuts::putsnobg((char*)"sub-tests failed: ", true, ColorPuts::YELLOW);
		cout << getSubTestsFailed() << endl;
	}
};

#endif
