/*  
*************************************************************************************

*************************************************************************************
*/
#ifndef MecanumDrive_h  
#define MecanumDrive_h
 
#include "Arduino.h" 
#include "Motor.h" 
#include "Dagu4Motor.h" 
#include "ControlSpeedStrategy.h"

class MecanumDrive  
{  

public:  
	/*
	 * C'tor
	 */
    MecanumDrive(); 
	
	void open(const int* pwmPin, const int* dirPin, const int* currPin, const int* encPin);
	
	void setSpeed(Motor motor, int speed)               { _motors[motor]->setSpeed(speed);              }  
	int getSpeed(Motor motor)                           { return _motors[motor]->getSpeed();            }
	
    void setMotorDirection(Motor motor, bool direction) { _motors[motor]->setMotorDirection(direction); } 
	bool getMotorDirection(Motor motor)                 { return _motors[motor]->getMotorDirection();   } 
	
	void setValue(Motor motor, const char value)        { _motors[motor]->setValue(value);              }
	/**
	 * Sets the values for the mecanumdrive using three values representing two joysticks.
	 * The range of the values is between [-100,100].  
	 */
	void setValue(const int ch1, const int ch3, const int ch4);
	
	int getCurrent(Motor motor) { return _motors[motor]->getCurrent(); }
	
	void setControlSpeedStrategy( void(*)(int&, int&, int&) );
	
	void correctSpeed();
	
	
private: 

	void normalizeValues(int& frontLeft, int& rearLeft, int& frontRight, int& rearRight);
	
private:  

	static const int threshold;

	char _value[4];
	
    Dagu4Motor* _motors[4];    
	
	void (* _controlSpeedStrategy)(int&, int&, int&);
};


#endif