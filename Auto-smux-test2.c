#pragma config(Hubs,  S1, HTMotor,  HTMotor,  HTMotor,  none)
#pragma config(Hubs,  S2, HTServo,  HTMotor,  HTServo,  none)
#pragma config(Sensor, S1,     ,               sensorI2CMuxController)
#pragma config(Sensor, S2,     ,               sensorI2CMuxController)
#pragma config(Sensor, S3,     accel,          sensorI2CHiTechnicAccel)
#pragma config(Sensor, S4,     gyro,           sensorI2CHiTechnicGyro)
#pragma config(Motor,  mtr_S1_C1_1,     motorLF,       tmotorTetrix, openLoop, reversed, driveLeft, encoder)
#pragma config(Motor,  mtr_S1_C1_2,     motorLR,       tmotorTetrix, openLoop, reversed, driveLeft, encoder)
#pragma config(Motor,  mtr_S1_C2_1,     motorRF,       tmotorTetrix, openLoop, driveRight, encoder)
#pragma config(Motor,  mtr_S1_C2_2,     motorRR,       tmotorTetrix, openLoop, driveRight, encoder)
#pragma config(Motor,  mtr_S1_C3_1,     motorArm1,     tmotorTetrix, openLoop, reversed)
#pragma config(Motor,  mtr_S1_C3_2,     motorArm2,     tmotorTetrix, openLoop)
#pragma config(Motor,  mtr_S2_C2_1,     motorLift1,    tmotorTetrix, openLoop, encoder)
#pragma config(Motor,  mtr_S2_C2_2,     motorLift2,    tmotorTetrix, openLoop, reversed, encoder)
#pragma config(Servo,  srvo_S2_C1_1,    servo_front_right,    tServoStandard)
#pragma config(Servo,  srvo_S2_C1_2,    servo_front_left,     tServoStandard)
#pragma config(Servo,  srvo_S2_C1_3,    servo_rear,           tServoStandard)
#pragma config(Servo,  srvo_S2_C1_4,    servo4,               tServoNone)
#pragma config(Servo,  srvo_S2_C1_5,    servo5,               tServoNone)
#pragma config(Servo,  srvo_S2_C1_6,    servo6,               tServoNone)
#pragma config(Servo,  srvo_S2_C3_1,    servo_tilt,           tServoStandard)
#pragma config(Servo,  srvo_S2_C3_2,    servo_trapdoor,       tServoStandard)
#pragma config(Servo,  srvo_S2_C3_3,    servo_spin,           tServoContinuousRotation)
#pragma config(Servo,  srvo_S2_C3_4,    servo_tilt2,          tServoStandard)
#pragma config(Servo,  srvo_S2_C3_5,    servo11,              tServoNone)
#pragma config(Servo,  srvo_S2_C3_6,    servo12,              tServoNone)
//*!!Code automatically generated by 'ROBOTC' configuration wizard               !!*//

//#pragma config(Motor,  mtr_S1_C4_1,     motorLift1,    tmotorTetrix, openLoop, encoder)
//#pragma config(Motor,  mtr_S1_C4_2,     motorLift2,    tmotorTetrix, openLoop, reversed, encoder)
//*!!Code automatically generated by 'ROBOTC' configuration wizard               !!*//

/////////////////////////////////////////////////////////////////////////////////////////////////////
//
//                           Tele-Operation Mode Code Template
//
// This file contains a template for simplified creation of an tele-op program for an FTC
// competition.
//
// You need to customize two functions with code unique to your specific robot.
//
/////////////////////////////////////////////////////////////////////////////////////////////////////

#include "hitechnic-accelerometer.h"
#include "lego-ultrasound.h"
#include "hitechnic-gyro.h"
#include "JoystickDriver.c"  //Include file to "handle" the Bluetooth messages.


//#define accel msensor_S3_2 //

//const tMUXSensor LEGOUS = msensor_S3_2;

/////////////////////////////////////////////////////////////////////////////////////////////////////
//
//                                    initializeRobot
//
// Prior to the start of tele-op mode, you may want to perform some initialization on your robot
// and the variables within your program.
//
// In most cases, you may not have to add any code to this function and it will remain "empty".
//
/////////////////////////////////////////////////////////////////////////////////////////////////////
void moveTilt (int tiltValue)
{
	servo[servo_tilt2] = 255 - tiltValue;
	wait10Msec(1);
	servo[servo_tilt] = tiltValue;
}


void initializeRobot()
{
	// Place code here to sinitialize servos to starting positions.
	// Sensors are automatically configured and setup by ROBOTC. They may need a brief time to stabilize.
	playSound(soundFastUpwardTones);
  //moveTilt(255);
  servo[servo_tilt2] = 0;
	wait10Msec(1);
	servo[servo_tilt] = 255;

	servo[servo_spin] = 127; // not moving
	servo[servo_trapdoor] = 160; //closed
	servo[servo_rear] = 30;  //up

	clearDebugStream();


	servo[servo_front_right] = 80;
	servo[servo_front_left] = 254;


	return;
}
// Create struct to hold accelerometer sensor data
tHTAC accelerometer;

float degreesOffHeading = 0;
int initialReading = SensorValue(gyro);

// drive stright using the gyro sensor
//   inputs = power (-100 to 100) and gain (higher gain makes for sharper corrections)

void driveStraight (float power, float gain) {
	int currentGyroReading = SensorValue(gyro) - initialReading;
	degreesOffHeading += (currentGyroReading * 0.01);
	// is negative drifting to left?
	float powerLeft = power - (gain * degreesOffHeading);
	if (powerLeft > 100) powerLeft = 100;
	if (powerLeft < -100) powerLeft = -100;
	float powerRight = power + (gain * degreesOffHeading);
	if (powerRight > 100) powerRight = 100;
	if (powerRight < -100) powerRight = -100;
	motor[motorRF] = powerRight;
	motor[motorRR] = powerRight;
	motor[motorLF] = powerLeft;
	motor[motorLR] = powerLeft;
	writeDebugStreamLine("gyro = %d start = %d degOffHeading = %f enc1 = %d powR = %f powL = %f\n",
	currentGyroReading,initialReading, degreesOffHeading, nMotorEncoder[motorRF], powerRight, powerLeft);
}

void turnToHeading (float heading) {
	int notdone = 1;
	while (notdone) {
		int currentGyroReading = SensorValue(gyro) - initialReading;
		degreesOffHeading += (currentGyroReading * 0.05); // same as sleep value
		if (degreesOffHeading > heading) {
			motor[motorRF] = 30;
			motor[motorRR] = 30;
			//motor[motorLF] = -30;
			//motor[motorLR] = -30;
			motor[motorLF] = 0;
			motor[motorLR] = 0;
			} else if (degreesOffHeading < heading) {
			motor[motorRF] = -30;
			motor[motorRR] = -30;
			//motor[motorLF] = 30;
			//motor[motorLR] = 30;
						motor[motorLF] = 0;
			motor[motorLR] = 0;
			}
			if (abs(degreesOffHeading-heading) < 0.5) notdone = 0;

			writeDebugStreamLine("gyro = %d start = %d degOffHeading = %f enc1 = %d \n",
				currentGyroReading,initialReading, degreesOffHeading, nMotorEncoder[motorRF]);
				sleep(50);
	}
				motor[motorRF] = 0;
			motor[motorRR] = 0;
			motor[motorLF] = 0;
			motor[motorLR] = 0;
}
// stop all drive motors

void allStop () {
	motor[motorRF] = 0;
	motor[motorRR] = 0;
	motor[motorLF] = 0;
	motor[motorLR] = 0;
}

// arm position

// Position values for accellerometer
// x = 0 horizontal
// x = 187 vertical
// x = -145 start position
// x = -180 lowest

float moveArmToPosition(float armPosition) {
if (!readSensor(&accelerometer)) {
      displayTextLine(4, "ERROR!!");
      writeDebugStreamLine("ERROR!\n");
    }
    int diff = armPosition - accelerometer.x;
    float upPower = -30.;
    if (diff <= 40) {

    upPower = -30. * (float) diff/40.;
  }
	writeDebugStreamLine("arm accel = %d\n",accelerometer.x);
	if (accelerometer.z > 0) {
	if (accelerometer.x < (armPosition-5.)) {
		motor[motorArm1] = (int) upPower; //up
		motor[motorArm2] = (int) upPower; //up
		writeDebugStreamLine("Up!\n");
		} else if (accelerometer.x > (armPosition+5.)) {
		motor[motorArm1] = 15; //down
		motor[motorArm2] = 15; //down
		writeDebugStreamLine("Down!\n");
		} else {
		motor[motorArm1] = 0;
		motor[motorArm2] = 0;
		writeDebugStreamLine("Stop!\n");
	}
}
	if (accelerometer.z < 0) {  // if something really messes up
		motor[motorArm1] = 40; //down
		motor[motorArm2] = 40; //down
		writeDebugStreamLine("Broken!\n");
	}
	float errorval = accelerometer.x - armPosition;
	return (errorval);
}


task main()
{

	// Initialise and configure struct and port
	initSensor(&accelerometer, accel);
	initializeRobot();
	//
	waitForStart();   // wait for start of tele-op phase

	// AndyMark Neverrest Encoder = 1120 tics per rotation
	// wheels = 4 in diameter
	// 1 rotation = 4 in * 3.14 = 12.56 in
	// tics / inch = 1120 / 12.56 = 89.2
	// there are 89.2 encoder ticks in one inch
	float ticksPerInch = 89.2;

	float power = -70; // negative is forward
	float gain = 50;

	//turnToHeading(-90.);
	//return;
	nMotorEncoder[motorRF]= 0;
	while ( nMotorEncoder[motorRF] > -(90*ticksPerInch)) {
		wait1Msec(10);
		driveStraight ( power, gain);
		moveArmToPosition(170.);
	}
	moveTilt (30);
	servo[servo_spin]=0;
	allStop ();
	motor[motorArm1] = 0; // make sub routine
	motor[motorArm2] = 0;
	wait1Msec(1000);
			servo[servo_front_right] = 240;
			servo[servo_front_left] = 100;
			wait1Msec(2000);
	nMotorEncoder[motorRF] = 0; //reset encoder
	power = 80;

	allStop();
	motor[motorArm1] = 0;
	motor[motorArm2] = 0;



	//turnToHeading (5) ;
	//initialReading = SensorValue(gyro);
//	while ( nMotorEncoder[motorRF] < (10*ticksPerInch)) {
	//	wait1Msec(10);
	//	driveStraight ( power, gain);
//		moveArmToPosition(160.);
	//}
	//allStop();

//	turnToHeading (25);
	//allstop();

	//initialReading = SensorValue(gyro);
	//while ( nMotorEncoder[motorRF] < (90*ticksPerInch)) {
		//wait1Msec(10);
	//	driveStraight ( power, gain);
//		moveArmToPosition(160.);
	//}
	servo[servo_spin]= 127;

	motor [motorLF] = 50;
	motor [motorLR] = 50;
	motor [motorRF] = 0;
	motor [motorRR] = 0;
  wait1Msec (1600);




	nMotorEncoder[motorRF]= 0;

	while ( nMotorEncoder[motorRF] < (115*ticksPerInch)) {
		motor [motorRF] = 30;
		motor [motorRR] = 30;
		motor [motorLF] = 30;
		motor [motorLR] = 30;
		moveArmToPosition(185.);
	}

		allStop ();

	/*
	while (moveArmToPosition(0.) < 5 || moveArmToPosition(0.) > -5) {}
	motor[motorArm1] = 0;
	motor[motorArm2] = 0;
	wait1Msec(2000);
	while (moveArmToPosition(90.) < 5 || moveArmToPosition(90.) > -5) {}
	motor[motorArm1] = 0;
	motor[motorArm2] = 0;
	wait1Msec(2000);
	while (moveArmToPosition(-145.) < 5 || moveArmToPosition(-145.) > -5) {}
	motor[motorArm1] = 0;
	motor[motorArm2] = 0;

	wait1Msec(5000);
	*/
}
/*
nMotorEncoder[motorLift1] = 0;
nMotorEncoder[motorLift2] = 0;
nMotorEncoderTarget[motorLift1] = -2000;
nMotorEncoderTarget[motorLift2] = -2000;
wait1Msec(50);
motor[motorLift1] = 60;
motor[motorLift2] = 60;
readSensor(&accelerometer);
while(nMotorRunState[motorLift1] != runStateIdle)
{ 		writeDebugStreamLine("accel X = %d Z = %d  enc1 = %d enc2 = %d\n",accelerometer.x, accelerometer.z, nMotorEncoder[motorLift1], nMotorEncoder[motorLift2]);

// This is an idle loop. The program waits until the condition is satisfied
}
//	motor[motorLift1] = 0; //Turn off motorB
//	motor[motorLift2] = 0; //Turn off motorC


// AndyMark Neverrest Encoder = 1120 tics per rotation
// wheels = 4 in diameter
// 1 rotation = 4 in * 3.14 = 12.56 in
// tics / inch = 1120 / 12.56 = 89.2

//nMotorEncoder[motorRF] = 0;


///////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////
////                                                   ////
////      Add your robot specific tele-op code here.   ////
////                                                   ////
///////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////
//motor[motorLF] = 30;
/*
short dist = 999; // initialize to something big (cm)
dist = USreadDist(LEGOUS);
while(dist > 20) {
// Read the current distance detected.
dist = USreadDist(LEGOUS);
motor[motorRF] = -20;
motor[motorRR] = -20;

motor[motorLF] = -20;
motor[motorLR] = -20;
writeDebugStreamLine("dist in cm = %d\n",dist);
}
motor[motorRF] = 0;

motor[motorRR] = 0;

motor[motorLF] = 0;
motor[motorLR] = 0;

}
*/
