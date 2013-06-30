/********************************************************************
Author: Qingfeng Xia  2003  
Copyright:  IEsensor.com    GPLv3
Description:   
*********************************************/
#include "DCMotor.h"
#include "Car.h"
#include <ctype.h>


// tested on Mega2560   
//  analogWrite() only apply to PWM-enabled pins: 
//PWM: 2 to 13 and 44 to 46. Provide 8-bit PWM output with the analogWrite() function. 
// but 2 and 3 should be seserved for external interrupt

#define LED_PIN 13

#define MOTER_enable_pin 10    // infact all motor are enabled by on board switch,  current is 70mA at 7V for two L298N
DCMotor rm_front(MOTER_enable_pin,29,27);  // first pin is enablePin, second and third is PWM
DCMotor lm_front(MOTER_enable_pin,25,23);
DCMotor rm_rear(MOTER_enable_pin,28,26);  // first pin is enablePin, second and third is PWM
DCMotor lm_rear(MOTER_enable_pin,24,22);

Car car(CarDriveMode::FD4W);  //  DC motor has no coast(sliding mode)
/////////////////////////////////////////////////////////////////////////////////////////
#define I2CLCD  0  //you need remove LiquidCrystal Lib of system, and install 
#define SONAR 0

#if SONAR   //SR04  5V power supply
  #include <Ultrasonic.h>   // Lib " HCSR04Ultrasonic"
  #define TRIGGER_PIN  33
  #define ECHO_PIN     35
  Ultrasonic ultrasonic(TRIGGER_PIN, ECHO_PIN);
#endif

#if I2CLCD 
   // include the library code for LCD 16X12,  MCP23XXX, 
  #include "Wire.h"
  #include "LiquidCrystal.h"    //adafruitLiquidCrystal
  // Connect via i2c, default address #0 (A0-A2 not jumpered)
  LiquidCrystal lcd(0);
#endif


boolean run;
void setup() {
    run = true;
    pinMode(LED_PIN, OUTPUT);
    
    Serial.begin(9600);  // blue tooth support only this rate
    Serial1.begin(115200);  //servo controller
    car.addLeftMotor(&lm_front);  //front wheel first!
    car.addRightMotor(&rm_front);
    car.addLeftMotor(&lm_rear);
    car.addRightMotor(&rm_rear);
    car.setup();
    
    car.enableMotors();
    test();
    help();
    
    //no setup for SR04
    #if I2CLCD 
      // set up the LCD's number of rows and columns: 
      lcd.begin(16, 2);
      // Print a message to the LCD.
      lcd.print("hello, world, Car setup is sucessful");
    #endif
    Serial.println("Car setup is sucessful");  // detect reset caused by program error 
}    

void test()
{
     
     car.forward(500);
     car.coast(500);   
     car.brake();
     Serial.println("Forward action is done");
     
     // it odes not work, why? using usb cable and try again 5V input is not given  using USB
     Serial1.print("#1P800T1000\r\n");  // 
     Serial1.print("#2P800T1000\r\n");  //

     car.backward(500);
     car.coast(500);
     car.brake();
     Serial.println("Backward action is done");
     
     
     Serial1.print("#1P700T1000\r\n");  // S1号舵机旋转到脉宽为800的位置，使用时间1000ms
     Serial1.print("#2P700T1000\r\n");  // S2号舵机旋转到脉宽为700的位置，使用时间1000ms
     
     
     /*  four wheel car turn left is hard to design!  */
     car.turnLeft(200);
     car.brake();
     delay(1000);
     car.turnRight(200);
     car.brake();
     Serial.println("Turn left and right");
}
   

void help()
{
  Serial.println("send single char to control car:  E(enable) D(disable) L R B F S(stop) C(coast or traveling)");
  Serial.println("send single char to control car:  H (help) T(Test)");
  Serial.println("send string to control servo:  ");
}

//command parse from serial read, E(enable) D(disable) L R B F S(stop) T(traveling)
void remoteControl()
{
      if (Serial.available())  // get remote control char,   
      {
        switch( toupper(Serial.read()) )  // Arduino read is a blocking op? 
        {
          case 'D':
             car.disableMotors();
             break;
          case 'E':
             car.enableMotors();
             break;
          case 'B':
             car.backward(500);
             break;
          case 'F':
             car.forward(500);
             break;
         case 'L':
             car.turnLeft(200);
             car.brake();
             break;
          case 'R':
             car.turnRight(200);
             car.brake();
             break;
          case 'S':
             car.brake();
             break;
          case 'C':
             car.coast(500);
             break;
          //----------------
          case 'T':
             test();
             break;
          case 'H':
            help();
            break;
          //----------------
          default: 
              break;  //do nothing
        }
      }
}

void showRange()
{
  #if SONAR
    float cmMsec, inMsec;
    long microsec = ultrasonic.timing();
  
    cmMsec = ultrasonic.convert(microsec, Ultrasonic::CM);
    inMsec = ultrasonic.convert(microsec, Ultrasonic::IN);
    Serial.print("MS: ");
    Serial.print(microsec);
    Serial.print(", CM: ");
    Serial.print(cmMsec);
    Serial.print(", IN: ");
    Serial.println(inMsec);
  #endif
  
  #if I2CLCD
    lcd.setCursor(0, 1);
    // print the number of seconds since reset:
    lcd.print(millis()/1000);
  
    lcd.setBacklight(HIGH);
    delay(500);
    lcd.setBacklight(LOW);
    //delay(500);
  #endif

}


void loop() {
     if(run)
     {
       //test();
       
       remoteControl();
       showRange();
      
       digitalWrite(LED_PIN, run);
       //car.disableMotors(); //
     }
    delay(1000);
    run = ~run; 
}


