 /********************************************************************
*  Author: Qingfeng Xia  2003  
Copyright:  IEsensor.com    GPLv3
Description:    Define Low Level H-Bridge L298N controlled motor
***********************************************************************/
 
#ifndef _DCMOTOR_H
#define _DCMOTOR_H

 
class DCMotor
{
private:
    int _enablePin, _pin1, _pin2;
    const char* _controllerName;
public :
    DCMotor(int enablePin, int pin1, int pin2);  // 
    const char* getControllerName();  // get  model name of controller 
    void setControllerName(const char* controllerName);  
    void setup();  //enable pin, may also check pin suport on the specific Arduino plaform, 
    
    void enable();            //on()
    void disable();
    void forward();
    void backward();
    void coast();
    void brake();
    void showPinsLevel();
};

#endif
