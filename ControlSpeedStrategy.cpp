#include "ControlSpeedStrategy.h"


const double ControlSpeedStrategy::ln101_100 = log(101.0)/100.0;

void ControlSpeedStrategy::exponentialStrategy(int& ch1, int& ch3, int& ch4) { 
	if (signbit(ch1) > 0) {
		ch1 = -1 * round(exp(ln101_100 * fabs(ch1)) - 1);
	} else {
		ch1 = round(exp(ln101_100 * fabs(ch1)) - 1);
	}
	if (signbit(ch3) > 0) {
		ch3 = -1 * round(exp(ln101_100 * fabs(ch3)) - 1);
	} else {
		ch3 = round(exp(ln101_100 * fabs(ch3)) - 1);
	}
	if (signbit(ch4) > 0) {
		ch4 = -1 * round(exp(ln101_100 * fabs(ch4)) - 1);
	} else {
		ch4 = round(exp(ln101_100 * fabs(ch4)) - 1);
	}
}