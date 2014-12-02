/*
 * motorDriver.h
 *
 *  Created on: Nov 19, 2014
 *      Author: Ian Goodbody
 *      Function: Provides necessary setup for robot motor control
 */

#ifndef MOTORDRIVER_H_
#define MOTORDRIVER_H_


// Left wheel correction factor, used in case the motors have different speeds to the same signal
#define LW_CORRECTION_FACTOR 1.0
#define MAX_DUTY_CYCLE 6000

// Left wheel control pins
#define LWEN 0x01
#define LWFS 0x02
#define LWBS 0x04

// Right wheel control pins
#define RWEN 0x08
#define RWFS 0x20
#define RWBS 0x10

#define FORWARD  0x01
#define BACKWARD 0x00

void initMotors();

void moveBothWheels(unsigned int dutyCycle, unsigned char direction);
void moveRightWheel(unsigned int dutyCycle, unsigned char direction);
void moveLeftWheel(unsigned int dutyCycle, unsigned char direction);

void stop();
void go();

void setBothWheels(unsigned int dutyCycle, unsigned char direction);
void setRightWheel(unsigned int dutyCycle, unsigned char direction);
void setLeftWheel(unsigned int dutyCycle, unsigned char direction);

#endif /* MOTORDRIVER_H_ */
