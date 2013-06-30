#include "DCMotor.h"
#include <Arduino.h>
////////////////////////////////////////////////////
DCMotor::DCMotor(int enablePin, int pin1, int pin2)
{
    _enablePin = enablePin;
    _pin1=pin1;
    _pin2=pin2;
    //
}

void DCMotor::setup()
{
     pinMode(_enablePin, OUTPUT);
     pinMode(_pin1, OUTPUT);
     pinMode(_pin2, OUTPUT);
}

const char* DCMotor::getControllerName()
{
    if (_controllerName != NULL)
    {
        return _controllerName;
    }
    else
    {
        return "Unknowned Controller";
    }
}
        
void DCMotor::setControllerName(const char* controllerName)
{
    _controllerName=controllerName;
}


//enable motors
 void DCMotor::enable()
{
 digitalWrite(_enablePin, HIGH);
}
  //disable motors
  void DCMotor::disable()
{
 digitalWrite(_enablePin, LOW);
}
 
 void DCMotor::showPinsLevel()
 {
   Serial.print("enable pin level: ");
   Serial.println(digitalRead(_enablePin));
   Serial.print("control pin #1: ");
   Serial.println(digitalRead(_pin1));
   Serial.print("control pin #2: ");
   Serial.println(digitalRead(_pin2));
 }
 
 //motor L controls
  void DCMotor::forward()
{
 digitalWrite(_pin1, HIGH);
 digitalWrite(_pin2, LOW);
}
 
   void DCMotor::backward()
{
 digitalWrite(_pin1, LOW);
 digitalWrite(_pin2, HIGH);
}

//coasting and braking
  void DCMotor::coast()
{
 digitalWrite(_pin1, LOW);
 digitalWrite(_pin2, LOW);
}
  void DCMotor::brake()
{
 digitalWrite(_pin1, HIGH);
 digitalWrite(_pin2, HIGH);
}
