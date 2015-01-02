#pragma config(Hubs,  S1, HTMotor,  HTMotor,  HTMotor,  none)
#pragma config(Hubs,  S2, HTServo,  HTMotor,  HTServo,  none)
#pragma config(Sensor, S1,     ,               sensorI2CMuxController)
#pragma config(Sensor, S2,     ,               sensorI2CMuxController)
#pragma config(Sensor, S3,     HTMUX3,         sensorI2CCustom)
#pragma config(Sensor, S4,     gyro,           sensorI2CHiTechnicGyro)
#pragma config(Motor,  mtr_S1_C1_1,     motorLF,       tmotorTetrix, PIDControl, reversed, driveLeft, encoder)
#pragma config(Motor,  mtr_S1_C1_2,     motorLR,       tmotorTetrix, PIDControl, reversed, driveLeft, encoder)
#pragma config(Motor,  mtr_S1_C2_1,     motorRF,       tmotorTetrix, PIDControl, driveRight, encoder)
#pragma config(Motor,  mtr_S1_C2_2,     motorRR,       tmotorTetrix, PIDControl, driveRight, encoder)
#pragma config(Motor,  mtr_S1_C3_1,     motorArm1,     tmotorTetrix, PIDControl)
#pragma config(Motor,  mtr_S1_C3_2,     motorArm2,     tmotorTetrix, PIDControl, reversed)
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
#pragma config(Servo,  srvo_S2_C3_4,    servo10,              tServoNone)
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


#define accel msensor_S3_1 //

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

void initializeRobot()
{
	// Place code here to sinitialize servos to starting positions.
	// Sensors are automatically configured and setup by ROBOTC. They may need a brief time to stabilize.
	playSound(soundFastUpwardTones);
	servo[servo_spin] = 127;

	return;
}


/////////////////////////////////////////////////////////////////////////////////////////////////////
//
//                                         Main Task
//
// The following is the main code for the tele-op robot operation. Customize as appropriate for
// your specific robot.
//
// Game controller / joystick information is sent periodically (about every 50 milliseconds) from
// the FMS (Field Management System) to the robot. Most tele-op programs will follow the following
// logic:
//   1. Loop forever repeating the following actions:
//   2. Get the latest game controller / joystick settings that have been received from the PC.
//   3. Perform appropriate actions based on the joystick + buttons settings. This is usually a
//      simple action:
//      *  Joystick values are usually directly translated into power levels for a motor or
//         position of a servo.
//      *  Buttons are usually used to start/stop a motor or cause a servo to move to a specific
//         position.
//   4. Repeat the loop.
//
// Your program needs to continuously loop because you need to continuously respond to changes in
// the game controller settings.
//
// At the end of the tele-op period, the FMS will autonmatically abort (stop) execution of the program.
//
/////////////////////////////////////////////////////////////////////////////////////////////////////

task main()
{
	// Create struct to hold accelerometer sensor data
	tHTAC accelerometer;

	// Initialise and configure struct and port
	initSensor(&accelerometer, accel);
	initializeRobot();
	//
	waitForStart();   // wait for start of tele-op phase
	float degreesOffHeading = 0;
	int initialReading = SensorValue(gyro);

	float power = -100;
	float gain = 10;
	motor[motorRF] = power;
	motor[motorRR] = power;
	motor[motorLF] = power;
	motor[motorLR] = power;

	while ( nMotorEncoder[motorRF] > -(48*89.2)) {
		wait1Msec(10);
		int currentGyroReading = SensorValue(gyro) - initialReading;
		degreesOffHeading += currentGyroReading * 0.01;

		float powerLeft = power - (gain * degreesOffHeading);
		float powerRight = power + (gain * degreesOffHeading);
		motor[motorRF] = powerRight;
		motor[motorRR] = powerRight;
		motor[motorLF] = powerLeft;
		motor[motorLR] = powerLeft;
		writeDebugStreamLine("gyro = %d start = %d degOffHeading = %f enc1 = %d\n",currentGyroReading,initialReading, degreesOffHeading, nMotorEncoder[motorRF]);


		/*
		if (degreesOffHeading > 0) {
		motor[motorRF] = -10;
		motor[motorRR] = -10;
		motor[motorLF] = -30;
		motor[motorLR] = -30;
		} else if (degreesOffHeading < 0) {
		motor[motorRF] = -30;
		motor[motorRR] = -30;
		motor[motorLF] = -10;
		motor[motorLR] = -10;
		} else {
		motor[motorRF] = -30;
		motor[motorRR] = -30;
		motor[motorLF] = -30;
		motor[motorLR] = -30;
		}
		*/
	}
	motor[motorRF] = 0;
	motor[motorRR] = 0;
	motor[motorLF] = 0;
	motor[motorLR] = 0;
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
