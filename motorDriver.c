/*
 * motorDriver.h
 *
 *  Created on: Nov 19, 2014
 *      Author: Ian Goodbody
 *    Function: Provides basic level robot motor instantiation and motor control functions
 */

/*
 * void initMotors
 * Inputs: none
 * Function: initalizes the motor control pins ect
 */

#include <msp430.h>
#include "motorDriver.h"

void initMotors()
{
	// Configure output pins
	P2DIR |= (RWEN|RWFS|RWBS|LWEN|LWFS|LWBS);
	P2OUT &= ~(RWEN|LWEN); // Shut off motors to start

    // Configure timer PWM mode
	TA1CTL |= TASSEL_2|ID_1|MC_1;
	TA1CCR0 = 10000;

	TA1CCTL1 |= OUTMOD_7;
	TA1CCR1 = 0x0000;

	TA1CCTL2 |= OUTMOD_7;
	TA1CCR2 = 0x0000;
}

/*
 * void moveBothWheels
 * Inputs: unsigned int dutyCycle: a number between 0 and 1000 specifying the PWM duty cycle for the motors
 *         unsighed char direction: specifies the direction of the wheels
 * Function: Dynamically rotate both wheels a single speed
 */
void moveBothWheels(unsigned int dutyCycle, unsigned char direction)
{
	P2OUT &= ~(RWEN|LWEN);
	TA1CTL &= ~(BIT4|BIT5);
	TA1R = 0x0000; // Reset the timer

	if (dutyCycle < MAX_DUTY_CYCLE)
		TA1CCR1 = TA1CCR2 = dutyCycle;
	else
		TA1CCR1 = TA1CCR2 = MAX_DUTY_CYCLE;

	if(direction == FORWARD)
	{
		P2SEL &= ~(LWBS|RWBS);
		P2OUT &= ~(LWBS|RWBS); // Backward signal set to ground
		P2SEL = (LWFS|RWFS); // Forward signal linked to timer
	}
	else if (direction == BACKWARD)
	{
		P2SEL &= ~(LWFS|RWFS);
		P2OUT &= ~(LWFS|RWFS); // Forward signal set to ground
		P2SEL |= (LWBS|RWBS); // Backward signal linked to timer
	}
	else // Somebody put in something erroneous
	{
		P2SEL &= ~0x2F; // Clear the select pins to outputs
		P2SEL2 &= ~0x2F;
		P2OUT &= ~(LWFS|LWBS|RWFS|RWBS); // Ground out all input signals
	}

	P2OUT |= (RWEN|LWEN);
	TA1CTL |= BIT4; // Restart timer
}

 /*
  * void moveRightWheel
  * Inputs: unsigned int dutyCycle: a number between 0 and 1000 specifying the PWM duty cycle for the motor
  *         unsighed char direction: specifies the direction of the wheel
  * Function: Dynamically rotates the right wheel
  */
void moveRightWheel(unsigned int dutyCycle, unsigned char direction)
{
	P2OUT &= ~(RWEN|LWEN);
	TA1CTL &= ~(BIT4|BIT5); // Shut off timer by setting memory control to zero
	TA1R = 0x0000; // Reset the timer

	if (dutyCycle < MAX_DUTY_CYCLE)
		TA1CCR2 = dutyCycle;
	else
		TA1CCR2 = MAX_DUTY_CYCLE;

	if(direction == FORWARD)
	{
		P2SEL &= ~RWBS;
		P2OUT &= ~RWBS; // Backward signal set to ground
		P2SEL |= RWFS; // Forward signal linked to timer
	}
	else if (direction == BACKWARD)
	{
		P2SEL &= ~RWFS;
		P2OUT &= ~RWFS; // Forward signal set to ground
		P2SEL |= RWBS; // Backward signal linked to timer
	}
	else // Somebody put in something erroneous
	{
		P2SEL &= ~0x2F; // Clear the select pins to outputs
		P2SEL2 &= ~0x2F;
		P2OUT &= ~(LWFS|LWBS|RWFS|RWBS); // Ground out all input signals
	}

	P2OUT |= RWEN;
	TA1CTL |= BIT4; // Restart timer
}

  /*
  * void moveLeftWheel
  * Inputs: unsigned int dutyCycle: a number between 0 and 1000 specifying the PWM duty cycle for the motor
  *         unsighed char direction: specifies the direction of the wheel
  * Function: Dynamically rotates the left wheel
  */
void moveLeftWheel(unsigned int dutyCycle, unsigned char direction)
{
	P2OUT &= ~(RWEN|LWEN);
	TA1CTL &= ~(BIT4|BIT5);
	TA1R = 0x0000; // Reset the timer

	if (dutyCycle < MAX_DUTY_CYCLE)
		TA1CCR1 = dutyCycle;
	else
		TA1CCR1 = MAX_DUTY_CYCLE;

	if(direction == FORWARD)
	{
		P2SEL &= ~LWBS;
		P2OUT &= ~LWBS; // Backward signal set to ground
		P2SEL |= LWFS; // Forward signal linked to timer
	}
	else if (direction == BACKWARD)
	{
		P2SEL &= ~LWFS;
		P2OUT &= ~(LWFS); // Forward signal set to ground
		P2SEL |= (LWBS); // Backward signal linked to timer
	}
	else // Somebody put in something erroneous
	{
		P2SEL &= ~0x2F; // Clear the select pins to outputs
		P2SEL2 &= ~0x2F;
		P2OUT &= ~(LWFS|LWBS|RWFS|RWBS); // Ground out all input signals
	}

	P2OUT |= LWEN;
	TA1CTL |= BIT4; // Restart timer
}

 /*
  * void stop
  * Inputs: None
  * Function: Disables the wheel drive signals
  */
void stop()
{
	P2OUT &= ~(RWEN|LWEN);
	TA1CTL &= ~(BIT4|BIT5);
	TA1R = 0x0000; // Reset the timer
}

/*
 * void go
 * Inputs: None
 * Function: Enables the wheel drive signals
 */
void go()
{
	P2OUT |= (RWEN|LWEN);
	TA1CTL |= BIT4; // Restart timer
}

/*
 * void setBothWheels
 * Inputs: unsigned int dutyCycle: a number between 0 and 1000 specifying the PWM duty cycle for the motors
 *         unsighed char direction: specifies the direction of the wheels
 * Function: Sets the wheel characteristics for both wheels
 */
void setBothWheels(unsigned int dutyCycle, unsigned char direction)
{
	if (dutyCycle < MAX_DUTY_CYCLE)
			TA1CCR1 = TA1CCR2 = dutyCycle;
		else
			TA1CCR1 = TA1CCR2 = MAX_DUTY_CYCLE;

		if(direction == FORWARD)
		{
			P2SEL &= ~(LWBS|RWBS);
			P2OUT &= ~(LWBS|RWBS); // Backward signal set to ground
			P2SEL = (LWFS|RWFS); // Forward signal linked to timer
		}
		else if (direction == BACKWARD)
		{
			P2SEL &= ~(LWFS|RWFS);
			P2OUT &= ~(LWFS|RWFS); // Forward signal set to ground
			P2SEL |= (LWBS|RWBS); // Backward signal linked to timer
		}
		else // Somebody put in something erroneous
		{
			P2SEL &= ~0x2F; // Clear the select pins to outputs
			P2SEL2 &= ~0x2F;
			P2OUT &= ~(LWFS|LWBS|RWFS|RWBS); // Ground out all input signals
		}
}

/*
 * void setRightWheel
 * Inputs: unsigned int dutyCycle: a number between 0 and 1000 specifying the PWM duty cycle for the motor
 *         unsighed char direction: specifies the direction of the wheel
 * Function: Sets the right wheel drive parameters
 */
void setRightWheel(unsigned int dutyCycle, unsigned char direction)
{
	if (dutyCycle < MAX_DUTY_CYCLE)
		TA1CCR2 = dutyCycle;
	else
		TA1CCR2 = MAX_DUTY_CYCLE;

	if(direction == FORWARD)
	{
		P2SEL &= ~RWBS;
		P2OUT &= ~RWBS; // Backward signal set to ground
		P2SEL |= RWFS; // Forward signal linked to timer
	}
	else if (direction == BACKWARD)
	{
		P2SEL &= ~RWFS;
		P2OUT &= ~RWFS; // Forward signal set to ground
		P2SEL |= RWBS; // Backward signal linked to timer
	}
	else // Somebody put in something erroneous
	{
		P2SEL &= ~0x2F; // Clear the select pins to outputs
		P2SEL2 &= ~0x2F;
		P2OUT &= ~(LWFS|LWBS|RWFS|RWBS); // Ground out all input signals
	}
}

/*
 * void moveLeftWheel
 * Inputs: unsigned int dutyCycle: a number between 0 and 1000 specifying the PWM duty cycle for the motor
 *         unsighed char direction: specifies the direction of the wheel
 * Function: Sets the left wheel drive parameters
 */
void setLeftWheel(unsigned int dutyCycle, unsigned char direction)
{
	if (dutyCycle < MAX_DUTY_CYCLE)
		TA1CCR1 = dutyCycle;
	else
		TA1CCR1 = MAX_DUTY_CYCLE;

	if(direction == FORWARD)
	{
		P2SEL &= ~LWBS;
		P2OUT &= ~LWBS; // Backward signal set to ground
		P2SEL |= LWFS; // Forward signal linked to timer
	}
	else if (direction == BACKWARD)
	{
		P2SEL &= ~LWFS;
		P2OUT &= ~(LWFS); // Forward signal set to ground
		P2SEL |= (LWBS); // Backward signal linked to timer
	}
	else // Somebody put in something erroneous
	{
		P2SEL &= ~0x2F; // Clear the select pins to outputs
		P2SEL2 &= ~0x2F;
		P2OUT &= ~(LWFS|LWBS|RWFS|RWBS); // Ground out all input signals
	}
}
