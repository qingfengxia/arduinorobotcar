/********************************************************************
*  Author: Qingfeng Xia  2003  
Copyright:  IEsensor.com    GPLv3
Description:   High level wheel car steering, composing DCMotor, Senors, ServoArm, BlueTooth remote control
                    (1) First stage,  two wheel driving car , 
                     There is no designated positive or negative pole for DC motor,  swapping pin1 and pin2 can made motor rotate reversely
                     (2) sensors:  using  PID for sensor fusion , 
                            a)  altrasound range sensors included 
                            b)  acceleration or angle speed 
                            c) Infrared receiver
                     (3) Control by Infrared remote or PC control via bluetooth 
                     (4)  usb Camera supervision using Raspberry Pi usb wifi
                     (5) voice command recogniation by raspberry Pi connected to internet
Souce codes hosting: 
***********************************************************************/

#ifndef _CAR_H
#define _CAR_H

#include "DCMotor.h"
//#include <Arduino.h>

/* Drivemode:  
                (1) 2 wheel, front drive,    "2WFD"
                (2) 2 wheel rear drive,  can share code with 2WFD
                (3) 4 wheel full drive, need differential speed control for turning
*/ 
namespace CarDriveMode {
    enum DriveMode
    {
        UNKNOWN=0,
        FD2W,  // currently implemention 
        RD2W,  // reverse the turning action of front driving
        FD4W,  // not implemented yet
    };
};

//#define NONBLOCKING   // keep pins level outside, disable delay(duration) inside each action, delay() will keep MCU busy 
#ifndef NONBLOCKING     // only Forward/backward and brake operation, not for turn left and right
    #define RETURN_COAST_AFTER_ACTION     //this cause error! program halt!
#endif
#define DefaultActionDuration 200     //   default parameter value may not supported by Arduino! 
//#define DEBUG
// except for brake and coast,    default car steering condition is coast()
// 0.5A at 7A when moving!  left and right turn does not work
//#define HAS_IDLER_MOTORS


class Car
{
private:
     #define MAX_DRIVER_PAIR 2
     DCMotor*  _motorL[MAX_DRIVER_PAIR];  // may use a list for multiple wheel drived cars+ total motors on the left
     int _count_motor_pair;   // this will be set
     int _i_motorL;
     int _i_motorR;
     DCMotor*  _motorR[MAX_DRIVER_PAIR];  // changed to FR, FL, or using array!
     // for four wheels driving, the car width is needed, 
   #ifdef HAS_IDLER_MOTORS 
     #define IDLER_NUMBER 2   // at lease one, can upto any number 
     DCMotor* _idler_motors[IDLER_NUMBER];  // only travelling, always in coast condition
     int _idler_count;
   #endif  
     enum CarDriveMode::DriveMode _mode;

public :
    Car(CarDriveMode::DriveMode mode);
    Car(CarDriveMode::DriveMode mode, DCMotor* lm,DCMotor* rm);  // only for 

    void addLeftMotor(DCMotor* m);
    void addRightMotor(DCMotor* m);
    #ifdef HAS_IDLER_MOTORS 
    void addIdlerMotor(DCMotor* m);
    #endif
    
    CarDriveMode::DriveMode getDriveMode(); 
    void setDriveMode(CarDriveMode::DriveMode mode);
    const char* getDriveModeName();  // return predefiend string
    
    //only for four driving wheels car
    void setCarWidth(int widthMM);   // left and right wheel pair width in milli metre
    void setCarLength(int lengthMM);   // front and rear wheel gap in milli metre

    bool check();   // check wheel configuration
    void setup();   // setup pinMode and default wheel state (coast)
    void enableMotors();  //on(),  depends on jumper,  it can be set as alwasy enabled
    void disableMotors();

    void forward(unsigned short milliseconds=DefaultActionDuration);  
    void backward(unsigned short milliseconds=DefaultActionDuration);
    void turnLeft(unsigned short milliseconds=DefaultActionDuration);
    void turnRight(unsigned short milliseconds=DefaultActionDuration);

    void coast(unsigned short milliseconds=DefaultActionDuration);  //sliding without power input from DC motor
    void brake();

};

#endif










 
