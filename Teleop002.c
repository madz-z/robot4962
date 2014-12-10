#pragma config(Hubs,  S1, HTMotor,  HTMotor,  HTMotor,  HTMotor)
#pragma config(Hubs,  S2, HTServo,  none,     none,     none)
#pragma config(Sensor, S1,     ,               sensorI2CMuxController)
#pragma config(Sensor, S2,     ,               sensorI2CMuxController)
#pragma config(Sensor, S4,     gyro,           sensorI2CHiTechnicGyro)
#pragma config(Motor,  mtr_S1_C1_1,     motorLF,       tmotorTetrix, openLoop, reversed, driveLeft, encoder)
#pragma config(Motor,  mtr_S1_C1_2,     motorLR,       tmotorTetrix, openLoop, reversed, driveLeft, encoder)
#pragma config(Motor,  mtr_S1_C2_1,     motorRF,       tmotorTetrix, openLoop, driveRight, encoder)
#pragma config(Motor,  mtr_S1_C2_2,     motorRR,       tmotorTetrix, openLoop, driveRight, encoder)
#pragma config(Motor,  mtr_S1_C3_1,     motorArm1,     tmotorTetrix, openLoop, encoder)
#pragma config(Motor,  mtr_S1_C3_2,     motorArm2,     tmotorTetrix, openLoop, reversed, encoder)
#pragma config(Motor,  mtr_S1_C4_1,     motorJ,        tmotorTetrix, openLoop, encoder)
#pragma config(Motor,  mtr_S1_C4_2,     motorK,        tmotorTetrix, openLoop, encoder)
#pragma config(Servo,  srvo_S2_C1_1,    servo_tilt,           tServoStandard)
#pragma config(Servo,  srvo_S2_C1_2,    servo_spin,           tServoContinuousRotation)
#pragma config(Servo,  srvo_S2_C1_3,    servo3,               tServoNone)
#pragma config(Servo,  srvo_S2_C1_4,    servo4,               tServoNone)
#pragma config(Servo,  srvo_S2_C1_5,    servo5,               tServoNone)
#pragma config(Servo,  srvo_S2_C1_6,    servo6,               tServoNone)
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

#include "JoystickDriver.c"  //Include file to "handle" the Bluetooth messages.


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
clearDebugStream();

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
	initializeRobot();
	//
	//  waitForStart();   // wait for start of tele-op phase

	while (true)
	{
		///////////////////////////////////////////////////////////
		///////////////////////////////////////////////////////////
		////                                                   ////
		////      Add your robot specific tele-op code here.   ////
		////                                                   ////
		///////////////////////////////////////////////////////////
		///////////////////////////////////////////////////////////
		//motor[motorLF] = 30;

		getJoystickSettings(joystick);
		int deadzone = 20;

		if (abs(joystick.joy1_y1) > abs(deadzone)) {
			playSound(soundBlip);
			displayBigTextLine(3, "%d, %d", joystick.joy1_y1/1.28, joystick.joy1_y2/1.28);
			motor[motorRF] = -joystick.joy1_y1/1.28;
			motor[motorRR] = -joystick.joy1_y1/1.28;
			} else {
			motor[motorRF] = 0;
			motor[motorRR] = 0;
		}

		if (abs(joystick.joy1_y2) > abs (deadzone)) {
			motor[motorLF] = -joystick.joy1_y2/1.28;
			motor[motorLR] = -joystick.joy1_y2/1.28;
			} else {
			motor[motorLF] = 0;
			motor[motorLR] = 0;
		}
		if (joystick.joy2_y1 > deadzone) {
			motor[motorArm1] = 30; //down
			motor[motorArm2] = 30; //down
			} else if (joystick.joy2_y1 < -deadzone) {
			motor[motorArm1] = -60; //up
			motor[motorArm2] = -60; //up
			} else {
			motor[motorArm1] = 0;
			motor[motorArm2] = 0;
		}





		if (joystick.joy2_y2 > deadzone) {
		servo[servo_tilt] = 130;
		} else if (joystick.joy2_y2 < -deadzone) {
		servo[servo_tilt] = 60;
		} else {
		servo[servo_tilt] = 120;
		}

		if (joy2Btn(1) == 1) {
			//playSound(soundFastUpwardTones);
			servo[servo_spin] = 0;  // clockwise?
		} else if (joy2Btn(2) == 1) {
			//playSound(soundFastUpwardTones);
			servo[servo_spin] = 255;  //
		} else {
			servo[servo_spin] = 127;
	  }
	  if (joy2Btn(3) == 1) {
	  	// go to position -- 1120 ticks is one rotation for AndyMark Neverest
	    //but divide by our 3:1 gear ratio, so 373.33 = 360 degrees
	    	nMotorEncoder[motorArm1] = 0;
	      nMotorEncoder[motorArm2] = 0;

	      nMotorEncoderTarget[motorArm1] = 43;
      	nMotorEncoderTarget[motorArm2] = 43;

      	motor[motorArm1] = 20;
      	motor[motorArm2] = 20;
      	sleep(1000);
      	writeDebugStreamLine("arm = %d\n",nMotorEncoder[motorArm1]);
    }
    	  if (joy2Btn(4) == 1) {
	  	// go to position -- 1120 ticks is one rotation for AndyMark Neverest
//   	nMotorEncoder[motorArm1] = 0;
//     nMotorEncoder[motorArm2] = 0;

//     nMotorEncoderTarget[motorArm1] = -43;
//    	nMotorEncoderTarget[motorArm2] = -43;

//    	motor[motorArm1] = -20;
//    	motor[motorArm2] = -20;

       while (nMotorEncoder[motorRF] < 1120) {
          motor[motorRF] = 50;
       }
          motor[motorRF] = 0;



//       sleep(2000);
      	//writeDebugStreamLine("arm = %d\n",nMotorEncoder[motorArm1]);
    }
		// motor[motorLF] = 20;
		/*
		servo[servo2]=127;
		servo[servo1]=0;
		sleep(1000);
		servo[servo2]=0;
		servo[servo1]=127;
		sleep(1000);
		float test = sensorValue[gyro];
		eraseDisplay();
		displayBigStringAt(8, 8, "test");
		*/
		// Insert code to have servos and motors respond to joystick and button values.

		// Look in the ROBOTC samples folder for programs that may be similar to what you want to perform.
		// You may be able to find "snippets" of code that are similar to the functions that you want to
		// perform.
    writeDebugStreamLine("RF RR LF LR = %d %d %d %d %d %d\n",nMotorEncoder[motorRF],nMotorEncoder[motorRR],nMotorEncoder[motorLF],nMotorEncoder[motorLR],nMotorEncoder[motorArm1],nMotorEncoder[motorArm2]);


	}
}
