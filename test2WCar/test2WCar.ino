/********************************************************************
Author: Qingfeng Xia  2003  
Copyright:  IEsensor.com    GPLv3
Description:   
*********************************************/
#include "DCMotor.h"
#include "Car.h"


// tested on Mega2560   
//  analogWrite() only apply to PWM-enabled pins: 
//PWM: 2 to 13 and 44 to 46. Provide 8-bit PWM output with the analogWrite() function. 
// but 2 and 3 should be seserved for external interrupt

#define LED_PIN 13
#define MOTER_enable_pin 10    // infact all motor are enabled by on board switch,  current is 70mA at 7V for two L298N
DCMotor lm(MOTER_enable_pin,26,28);  // first pin is enablePin, second and third is PWM
DCMotor rm(MOTER_enable_pin,22,24);
DCMotor l_idler(MOTER_enable_pin,27,29);  // first pin is enablePin, second and third is PWM
DCMotor r_idler(MOTER_enable_pin,23,25);

Car car(CarDriveMode::FD2W);  //  DC motor has no coast(sliding mode)

 
boolean run;
void setup() {
    run = true;
    pinMode(LED_PIN, OUTPUT);
    
    Serial.begin(115200);
    car.addLeftMotor(&lm);
    car.addRightMotor(&rm);
    car.addIdlerMotor(&l_idler);
    car.addIdlerMotor(&r_idler);
    car.setup();
}                                                                                     

//command parse from serial read, rename the loop() to test()
void loop() {
     if(run)
     {
     car.enableMotors();
     
     car.forward(1000);
     car.coast(500);
     car.brake();
     Serial.println("Forward action is done!");

     car.backward(1000);
     car.coast(500);
     car.brake();
     Serial.println("Backward action is done!");
     
     /*
     car.turnLeft(500);
     car.brake();
     car.turnRight(500);
     car.brake();
     Serial.println("Turn left and right");
     */
     
     
     car.disableMotors(); //

     
     }
    delay(2000);
    run = ~run; 
}
