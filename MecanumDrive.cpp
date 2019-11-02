#include "Arduino.h"  
#include "MecanumDrive.h"


const int MecanumDrive::threshold = 10;

MecanumDrive::MecanumDrive():
	_controlSpeedStrategy( &(ControlSpeedStrategy::linearStrategy))
{  	
	memset(_value,0, sizeof(_value));
}

void MecanumDrive::open(const int* pwmPin, const int* dirPin, const int* currPin, const int* encPin) {
	_motors[FRONT_LEFT] = new Dagu4Motor(FRONT_LEFT,pwmPin[0],dirPin[0],currPin[0],encPin[0],true);
	_motors[REAR_LEFT] = new Dagu4Motor(REAR_LEFT,pwmPin[1],dirPin[1],currPin[1],encPin[1]);
	_motors[FRONT_RIGHT] = new Dagu4Motor(FRONT_RIGHT,pwmPin[2],dirPin[2],currPin[2],encPin[2],true);
	_motors[REAR_RIGHT] = new Dagu4Motor(REAR_RIGHT,pwmPin[3],dirPin[3],currPin[3],encPin[3]);
}

void MecanumDrive::setControlSpeedStrategy( void(* fnc)(int&, int&, int&) ) {
	MecanumDrive::_controlSpeedStrategy = fnc;
}
 
void MecanumDrive::normalizeValues(int& frontLeft, int& rearLeft, int& frontRight, int& rearRight) {
	int max = abs(frontLeft);
	
	if (abs(rearLeft) > max) {
		max = abs(rearLeft);
	}
	
	if (abs(frontRight) > max) {
		max = abs(frontRight);
	}

	if (abs(rearRight) > max) {
		max = abs(rearRight);
	}

	if (max > 100) {
		frontLeft  = frontLeft  * 100 / max;
		rearLeft   = rearLeft   * 100 / max;
		frontRight = frontRight * 100 / max;
		rearRight  = rearRight  * 100 / max;
	}
}

void MecanumDrive::setValue(const int ch1, const int ch3, const int ch4) 
{
	int tmpCh1 = ch1;
	int tmpCh3 = ch3;
	int tmpCh4 = ch4;
	
	// _controlSpeedStrategy(tmpCh1,tmpCh3,tmpCh4);
	
	if (abs(tmpCh1) < threshold) {
		tmpCh1 = 0;
	}
	if (abs(tmpCh3) < threshold) {
		tmpCh3 = 0;
	}
	if (abs(tmpCh4) < threshold) {
		tmpCh4 = 0;
	}
	
	int frontLeft = tmpCh3 + tmpCh1 / 2 + tmpCh4;
    int rearLeft = tmpCh3 + tmpCh1 / 2 - tmpCh4;
	int frontRight = tmpCh3 - tmpCh1 / 2 - tmpCh4;
	int rearRight = tmpCh3 - tmpCh1 / 2 + tmpCh4;
	
	normalizeValues(frontLeft,rearLeft,frontRight,rearRight);
	
	_value[FRONT_LEFT] = frontLeft;
	_value[REAR_LEFT] = rearLeft;
	_value[FRONT_RIGHT] = frontRight;
	_value[REAR_RIGHT] = rearRight;
	
	for (int i = 0; i < 4; i++) {
		setValue(static_cast<Motor>(i),_value[i]);
	}
}

void MecanumDrive::correctSpeed() {
		
	unsigned long lastPulses[4];
	for (int i = 0; i < 4; i++) {
		lastPulses[i] =  _motors[i]->getLastPulse();
	}
	
	// the reference motor is the motor with the highest speed, means pwm, means pulse count
	int referenceMotor = FRONT_LEFT;
	int max = abs(_value[FRONT_LEFT]);
	
	for (int i = 1; i < 4; i++) {
		if (abs(_value[i]) >= max) {
			if (lastPulses[i] > lastPulses[referenceMotor]) {
				max = abs(_value[i]);
				referenceMotor = i;
			}
		}
	}
	

	if (max == 0) {
		return;
	}
	
	unsigned long referencePulse = _motors[referenceMotor]->getLastPulse();
	unsigned long targetPulse;
	for (int i = 0; i < 4; i++){
		targetPulse = abs(referencePulse * _value[i] / max);
		if (targetPulse < lastPulses[i]) {
			_motors[i]->decSpeed();
		} else if (targetPulse > lastPulses[i]) {
			_motors[i]->incSpeed();
		}
	}
}