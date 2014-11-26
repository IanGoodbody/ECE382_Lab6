/*
 * RC.c
 *
 *  Created on: Nov 24, 2014
 *      Author: C16Ian.Goodbody
 */

#include <msp430.h>
#include "motorDriver.h"
#include "remoteInterface.h"
void waitMiliseconds(unsigned short time)
{

    TA0CTL = TASSEL_2|ID_3|MC_1;
	TA0CTL &= ~(BIT4|BIT5);
    TA0CCR0 = 1000; // Setting found using Oscillosope and trial and error generator
    TA0CTL &= ~TAIFG;
    TA0R = 0x0000;
    TA0CTL |= BIT4;

    while(time != 0)
    {
    	if(TA0CTL & TAIFG)
    	{
    		time--;
    		TA0CTL &= ~TAIFG;
    	}
    }
}

void forward()
{
	stop();
	waitMiliseconds(500);
	setLeftWheel(5070,FORWARD);
	setRightWheel(5000, FORWARD);
	go();
	waitMiliseconds(2000);
}

void backward()
{
	stop();
	waitMiliseconds(500);
	setLeftWheel(5070,BACKWARD);
	setRightWheel(5000, BACKWARD);
	go();
	waitMiliseconds(2000);
}

void longLeft()
{
	stop();
	waitMiliseconds(500);
	setRightWheel(5260,FORWARD);
	setLeftWheel(5090, BACKWARD);
	go();
	waitMiliseconds(450);
}

void longRight()
{
	stop();
	waitMiliseconds(500);
	setLeftWheel(5090,FORWARD);
	setRightWheel(5260, BACKWARD);
	go();
	waitMiliseconds(450);
}

void shortLeft()
{
	stop();
	waitMiliseconds(500);
	moveRightWheel(5000, FORWARD);
	waitMiliseconds(600);
}

void shortRight()
{
	stop();
	waitMiliseconds(500);
	moveLeftWheel(5070, FORWARD);
	waitMiliseconds(600);
}

int main(void)
{
    WDTCTL = WDTPW | WDTHOLD;	// Stop watchdog timer
    IFG1=0;

	BCSCTL1 = CALBC1_8MHZ;
	DCOCTL = CALDCO_8MHZ;

    initMotors();
    initIRSensor();
    moveBothWheels(3500, FORWARD);
    waitMiliseconds(500);
    stop();

    P1DIR |= BIT5;
    P1OUT |= BIT5;

    P1DIR |= BIT2;
    P1SEL |= BIT2;

	remoteCode packet;
    __enable_interrupt();
	TA0CTL &= ~TAIE;


    while(1)
    {
    	if(packetReadFlags & ERROR_FLG)
    	{
    		resetSensor();
    	}
    	else if(packetReadFlags & PACKET_IN_FLG)
    	{
    		__disable_interrupt();
    		P1OUT &= ~BIT5;
    		packet = translatePattern();
    		switch(packet)
    		{
    		case UP_BUTTON:
    			forward();
    			break;
    		case DOWN_BUTTON:
    			backward();
    			break;
    		case RIGHT_BUTTON:
    			longRight();
    			break;
    		case LEFT_BUTTON:
    			longLeft();
    			break;
    		case BUTTON_1:
    			shortLeft();
    			break;
    		case BUTTON_2:
    			shortRight();
    			break;
    		}
    		initIRSensor();
    		P1OUT |= BIT5;
    		__enable_interrupt();
    	}
    }
}
