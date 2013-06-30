
#include <Arduino.h>
#include "DCMotor.h"
#include "Car.h"


/////////////////////////////////////////////////////////////////////
Car::Car(CarDriveMode::DriveMode mode)
{
    //_idler_count=0;
    //_count_motor_pair=0;   // depends on drivemode and set by setDriveMode()
    setDriveMode(mode);
    _i_motorL=0;
    _i_motorR=0;
    _motorL[0]=NULL;  // second array element is not clear as NULL
    _motorR[0]=NULL;
}

Car::Car(CarDriveMode::DriveMode mode, DCMotor* lm, DCMotor* rm)
{
    setDriveMode(mode);
    if (lm != NULL && rm != NULL)
    {
      _motorL[0]=lm; 
      _i_motorL=1;
      _motorR[0]=rm;
      _i_motorR=1;
    }
    else
    {
      Serial.println("Error pointer to left or right motor is NULL");
    }
}

void Car::addLeftMotor( DCMotor* m)
{
    if (m==NULL)
         Serial.println("Error NULL pointer is passed as left motor");
    if (_i_motorL < _count_motor_pair)
    {
        _motorL[_i_motorL]=m;
        _i_motorL++;
    }
    else
    {
        Serial.println("Error  no more motor can be added to this drive mode");
        Serial.println(_i_motorL);
        Serial.println(_count_motor_pair);
    }
}

void Car::addRightMotor(DCMotor* m)
{
    if (m==NULL)
         Serial.println("Error NULL pointer is passed as right motor");
    if (_i_motorR < _count_motor_pair)
    {
        _motorR[_i_motorR]=m;
        _i_motorR++;
    }
    else
        Serial.println("Error  no more motor can be added to this drive mode");
}

 CarDriveMode::DriveMode Car::getDriveMode()
{
    return _mode;
}

void Car::setDriveMode(CarDriveMode::DriveMode mode)
{
    _mode=mode;
    #ifdef HAS_IDLER_MOTORS 
      _idler_count=0;
    #endif
    switch(_mode)
    {
         case CarDriveMode::FD2W:
            _count_motor_pair=1;
            break;
        case CarDriveMode::RD2W:
            _count_motor_pair=1;
            break;
        case CarDriveMode::FD4W:
            _count_motor_pair=2;
            break;
        default:
            _count_motor_pair=1;
             // "Unknowned drive mode";
    }
}

const char* Car::getDriveModeName()
{
    switch(_mode)
    {
        case CarDriveMode::FD2W:
            return "Front drive by two wheel";
            break;
        case CarDriveMode::RD2W:
            return "Rear drive by two wheel";
            break;
        case CarDriveMode::FD4W:
            return "Full drive by four wheel";
            break;
        default:
            return  NULL;  // "Unknowned drive mode";
    }

}

#ifdef HAS_IDLER_MOTORS 
 void Car::addIdlerMotor(DCMotor* m)
 {
   if (_idler_count<IDLER_NUMBER  && m!= NULL)
   {
     _idler_motors[ _idler_count] = m;
     _idler_count++;
   }
   else
   {
      Serial.println("Too many idler motor for this car, or NULL pointer");  //debug info
      return;
   }
 }
#endif

bool Car::check()
{
  if (getDriveModeName() == NULL)
  {
     Serial.println("Car driving mode is not set properly, please check");
     return false;
  }
   if (_i_motorL<_count_motor_pair || _i_motorR<_count_motor_pair )
   {
     Serial.println("Not all left or right driving motors or wheels are added to this car");
     return false;
   }
   #ifdef HAS_IDLER_MOTORS 
   if (_idler_count<IDLER_NUMBER)
   {
     Serial.println("Not all idling motor or wheels are added to this car");
     return false;
   }
   #endif
   return true;
}

void Car::setup()
{
    if (check() )
    {
      for (int i=0; i<_count_motor_pair ; i++)
      {
        _motorL[i]->setup();
        _motorR[i]->setup();
      }
      enableMotors();
      #ifdef HAS_IDLER_MOTORS 
      for (int i=0; i<IDLER_NUMBER ; i++)
      {
          //_idler_motors[i]->enable();
          _idler_motors[i]->setup();
          _idler_motors[i]->coast();
      }
      #endif
      #ifdef DEBUG
          //Serial.println("Car setup is sucessful");
      #endif
    }
    else
    {
      Serial.println("Self testing failed before setup, please check car mode and motors configuration ");
    }
}

void Car::enableMotors()
{
     for (int i=0; i<_count_motor_pair ; i++)
     {
       _motorL[i]->enable();
       _motorR[i]->enable();
     }
     #ifdef HAS_IDLER_MOTORS 
     for (int i=0; i<IDLER_NUMBER ; i++)
     {
       _idler_motors[i]->enable();
     }
     #endif
    
}
 
void Car::disableMotors()
{
     for (int i=0; i<_count_motor_pair ; i++)
     {
       _motorL[i]->disable();
       _motorR[i]->disable();
     }
      #ifdef HAS_IDLER_MOTORS 
     for (int i=0; i<IDLER_NUMBER ; i++)
     {
       _idler_motors[i]->disable();
     }
     #endif
}
 
void Car::forward(unsigned short milliseconds)
{
     for (int i=0; i<_count_motor_pair ; i++)
     {
       _motorL[i]->forward();
       _motorR[i]->forward();
        #ifdef DEBUG
         Serial.println("Immediately after fwd action");
         _motorL[0]->showPinsLevel();
         _motorR[0]->showPinsLevel();
       #endif
     }
     
     //Serial.println("Idler motor:");
     //_idler_motors[0]->showPinsLevel();
     
    #ifndef NONBLOCKING
    if (milliseconds>0)
    {
        delay(milliseconds);
        #ifdef  RETURN_COAST_AFTER_ACTION
        coast(5);  //default car steering condition is coast()
        #endif
    }
    #endif 
    
    #ifdef DEBUG
     Serial.println("After fwd act with delay front wheel");
     _motorL[0]->showPinsLevel();
     _motorR[0]->showPinsLevel();
    #endif
    
}
 
void Car::backward(unsigned short  milliseconds)
{
     for (int i=0; i<_count_motor_pair ; i++)
     {
       _motorL[i]->backward();
       _motorR[i]->backward();
       #ifdef DEBUG
         Serial.println("Immediately after backward action");
         _motorL[0]->showPinsLevel();
         _motorR[0]->showPinsLevel();
       #endif
     }
    #ifndef NONBLOCKING
    if (milliseconds>0)
    {
       delay(milliseconds);
        #ifdef  RETURN_COAST_AFTER_ACTION
        coast(5);  //default car steering condition is coast()
        #endif
    }
    #endif 
}
 
void Car::turnLeft(unsigned short  milliseconds)
{
  // distinguish drivemode, four wheel driving is not implemented
    if ( _mode ==  CarDriveMode::FD2W )
    {
       _motorR[0]->forward();
       _motorL[0]->backward();
    }
    else if (_mode ==  CarDriveMode::RD2W )
    {
       _motorL[0]->forward();
       _motorR[0]->backward();
    }
    else if (_mode ==  CarDriveMode::FD4W )  // full wheel
    {
       _motorL[0]->backward();
       _motorR[0]->forward();   
       _motorL[1]->backward();   // coast is not working with this kind of motor!
       _motorR[1]->forward();
       // return;
    }
    else
    {
        // debug ,shoud not get here
        Serial.println("Trun left is not implemented for this drive mode");
        Serial.println(getDriveModeName());
    }
    //#ifndef NONBLOCKING
    if (milliseconds>0)
    {
        delay(milliseconds);
        //#ifdef  RETURN_COAST_AFTER_ACTION
        //coast(5);  //default car steering condition is coast()
        //#endif
    }
    //#endif 
}
 
void Car::turnRight(unsigned short  milliseconds)
{
    if ( _mode ==  CarDriveMode::FD2W )
    {
       _motorL[0]->forward();
       _motorR[0]->backward();
    }
    else if (_mode ==  CarDriveMode::RD2W )
    {
       _motorR[0]->forward();
       _motorL[0]->backward();
    }
    else if (_mode ==  CarDriveMode::FD4W )  // full wheel
    {
        // to do ! rear_right do not move but coast
        // method 1:  left front motor run double time(width/length=1), using PWM is smooth
        //method 2: 
       _motorR[0]->backward();
       #ifdef DEBUG
         Serial.println("pin level for right front motor in turn right");
         _motorL[0]->showPinsLevel();
         _motorL[1]->showPinsLevel();
       #endif
       _motorL[0]->forward();   
       _motorR[1]->backward();   // coast is not working with this kind of motor!
       _motorL[1]->forward();
       Serial.println("Trun right for four wheel car");
    }
    else
    {
        // debug ,shoud not get here
        Serial.println("Trun right is not implemented for this drive mode");
        Serial.println(getDriveModeName());
    }
    //#ifndef NONBLOCKING, turn left or right should be blocking operation
    if (milliseconds>0)
    {
       delay(milliseconds);
    }
    //endif
}
 
void Car::coast(unsigned short milliseconds)
{
    #ifdef DEBUG
    Serial.println("before car coasts");   // has been called many times ?!
    #endif
     for (int i=0; i<_count_motor_pair ; i++)
     {
       _motorL[i]->coast();
       _motorR[i]->coast();
     }
     
    #ifndef NONBLOCKING
    if (milliseconds>0)
    {
        delay(milliseconds);
    }
    #endif 
    #ifdef DEBUG
    Serial.println("car has coasted");
    #endif
}
 
void Car::brake()
{
     for (int i=0; i<_count_motor_pair ; i++)
     {
       _motorL[i]->brake();
       _motorR[i]->brake();
     }
     delay(5);
    #ifdef DEBUG
    Serial.println("car has braked");
    #endif
     //should not return to coast() until explicitly told!
    //idler motor is left as coast() 
}

