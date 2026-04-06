// Meow meow kitty cat
// imports
#include <Servo.h>

////// VARS //////
// Servos
Servo servo1_360;
Servo servo2_180;
Servo servo3_360;
Servo servo4_180;
int s1pin = 5;
int s2pin = 3;
int s3pin = 11;
int s4pin;
// LEDs
int rgbled_Rpin = 11;
int rgbled_Gpin = 9;
int rgbled_Bpin = 10;

//hi hello//


void setup() {
  ////// INITALIZE CONNECTIONS //////
  Serial.begin(9600);
  // Servos
  servo1_360.attach(s1pin);
  servo2_180.attach(s2pin);
  servo3_360.attach(s3pin);
  servo4_180.attach(s4pin);
  // LEDs
  pinMode(rgbled_Rpin, OUTPUT);
  pinMode(rgbled_Gpin, OUTPUT);
  pinMode(rgbled_Bpin, OUTPUT);


  //OpenDoor(2000, servo3_360);
  //delay(1000);
  //CloseDoor(2000, servo3_360);

  //servo2.write(70);
  //delay(1000);
  //servo2.write(0);

}

void loop() {

}

// opens door for servo s taking x seconds, with set speed
void OpenDoor(int amt, Servo s){
  s.write(80);
  delay(amt);
  s.write(95);
}
// closes door for servo s taking x seconds, with set speed
void CloseDoor(int amt, Servo s){
  s.write(100);
  delay(amt);
  s.write(90);
}
