# Lab 6
## Robot Control System

The objective for lab 6 is to use the MSP430 to operate directional controls for a small robot. The major design 
challenge for this lab is interfacing the microcontroler with the two robot driving motors to allow the robot to move
forward, backward, and turn in either direction. The interface itself will require a control circuit which can switch 
the direction of voltage across the motors to allow for forward and backward movement, and a software package with the 
necessary MSP430 pin initiation and movement functions.

### Prelab

#### Preliminary design

##### Driver Circuit

One of the main goals of any design project is to give the end user as much easy flexible control as possible. For this
robot it would be to have independent speed and direction control for each wheel. In order to realize this design, I 
opted for using only one motor driver gate per wheel, with the reverse direction implmented with a NMOS transistor 
switching network. A rough design of the driver circuit is shown below. In addition to the motor control the schematic
also includes the IR sensor, the power voltage regulator and decoupling unit, and a reset button. This circuit is 
slightly easier to work with on the software side as each wheel is controlled by a direction pin (LWFS or RWFS) and 
a foreard and backward driver signal. Later implementations had only one drive signal per wheel and one direction
bit for each wheel. Steps involved in setting the direction would be to set the drive PWM signal to constant low, 
configure the direction pins for each wheel, then reseting the duty cycles to the desired values for each wheel.

A second design was accomplished by wiring the two motor leads to the output pins on each side of the motor driver
chip. This design makes use of the two enable pins on the SN754410 driver to turn each wheel on and off whilst 
the wheel conditions are being set. Unlike the first design it requires drive pins for both forward and backward
directions on each wheel. Setting either the forward or backward drive signal to a constant low, then modulating 
it's compliment drive signal will give the user control in both forward and backward directions. This driver circuit 
is only slightly more complex for the user to implement in code (with an extra drive pin setting for each wheel), and 
far easier to implement on the limited space of the robot breadboard (although I did do it and it was beautiful). 


#### Software package

The software package will be realized according to the attached header draft. The function prototype code and
supporting comments are given below. The software package will have two sets of functions, one which can dynamically 
change whe wheel parameters (disables, rewrites, then enables) within the function, and another set that should be 
enveloped by stop() and go() methods that allows the user to manually set the different parameters.

```c
// Left wheel correction factor, used in case the motors have different speeds to the same signal 
#define LW_CORRECTION_FACTOR 1.0
#define MAX_DUTY_CYCLE 600

// Left wheel control pins
#define LWEN 0x04
#define LWFS 0x02
#define LWBS 0x01

// Right wheel control pins
#define RWEN 0x08
#define RWFS 0x10
#define RWBS 0x20

/*
 * void moveBothWheels
 * Inputs: unsigned int dutyCycle: a number between 0 and 1000 specifying the PWM duty cycle for the motors
 *         unsighed char direction: specifies the direction of the wheels
 * Function: Dynamically rotate both wheels a single speed
 */       
void moveBothWheels(unsigned int dutyCycle, unsighed char direction);
 
 /*
  * void moveRightWheel
  * Inputs: unsigned int dutyCycle: a number between 0 and 1000 specifying the PWM duty cycle for the motor
  *         unsighed char direction: specifies the direction of the wheel
  * Function: Dynamically rotates the right wheel
  */
void moveRightWheel(unsigned int dutyCycle, unsigned char direction);
  
  /*
  * void moveLeftWheel
  * Inputs: unsigned int dutyCycle: a number between 0 and 1000 specifying the PWM duty cycle for the motor
  *         unsighed char direction: specifies the direction of the wheel
  * Function: Dynamically rotates the left wheel
  */
void moveLeftWheel(unsigned int dutyCycle, unsigned char direction);
  
 /*
  * void stop
  * Inputs: None
  * Function: Disables the wheel drive signals
  */
void stop();
  
 /*
  * void go
  * Inputs: None
  * Function: Enables the wheel drive signals
  */
void go(); 
  
/*
 * void setBothWheels
 * Inputs: unsigned int dutyCycle: a number between 0 and 1000 specifying the PWM duty cycle for the motors
 *         unsighed char direction: specifies the direction of the wheels
 * Function: Sets the wheel characteristics for both wheels 
 */       
void setBothWheels(unsigned int dutyCycle, unsighed char direction);
 
 /*
  * void setRightWheel
  * Inputs: unsigned int dutyCycle: a number between 0 and 1000 specifying the PWM duty cycle for the motor
  *         unsighed char direction: specifies the direction of the wheel
  * Function: Sets the right wheel drive parameters
  */
void setRightWheel(unsigned int dutyCycle, unsigned char direction);
  
  /*
  * void moveLeftWheel
  * Inputs: unsigned int dutyCycle: a number between 0 and 1000 specifying the PWM duty cycle for the motor
  *         unsighed char direction: specifies the direction of the wheel
  * Function: Sets the left wheel drive parameters
  */
void setLeftWheel(unsigned int dutyCycle, unsigned char direction);
```

