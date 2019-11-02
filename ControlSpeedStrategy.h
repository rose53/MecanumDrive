/*  
*************************************************************************************

*************************************************************************************
*/
#ifndef ControlSpeedStrategy_h  
#define ControlSpeedStrategy_h
 
#include "Arduino.h" 


class ControlSpeedStrategy  
{  
public:  

	static void linearStrategy(int& ch1, int& ch3, int& ch4) { }
	static void exponentialStrategy(int& ch1, int& ch3, int& ch4);
	
private:  

	static const double ln101_100;	
};


#endif