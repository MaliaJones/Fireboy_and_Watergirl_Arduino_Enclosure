// imports
#include <Servo.h>

////// VARS //////
// Servos
Servo servo1_360;
Servo servo2_180;
Servo servo3_360;
Servo servo4_180;
int s1pin = 12;
int s2pin = 3;
int s3pin = 11;
int s4pin = 4;
// LEDs
int rgbled_Rpin = 11;
int rgbled_Gpin = 9;
int rgbled_Bpin = 10;
// Copper Tape Switches
int switch2pin = A1;


// Other
int prevState = LOW;
int currState = LOW;




void setup() {
  ////// INITALIZE CONNECTIONS //////
  Serial.begin(9600);
  // Servos
  servo1_360.attach(s1pin);
    servo1_360.write(90);
    delay(1000);
  servo2_180.attach(s2pin);
  servo3_360.attach(s3pin);
  servo4_180.attach(s4pin);
  // LEDs
  pinMode(rgbled_Rpin, OUTPUT);
  pinMode(rgbled_Gpin, OUTPUT);
  pinMode(rgbled_Bpin, OUTPUT);
  // Copper Tape Switches
  pinMode(switch2pin, INPUT);

  CloseDoor(200, servo1_360);


  //servo2.write(70);
  //delay(1000);
  //servo2.write(0);

}

void loop() {
  // Room2 switches
  currState = digitalRead(switch2pin);
  if (prevState != currState){
    if (currState == HIGH){
      Serial.println("switch2: HIGH");
      OpenDoor(1800, servo1_360);
      delay(2000);
    } else{
      Serial.println("switch2: LOW");
      CloseDoor(1800, servo1_360);
      delay(2000);
    }
    prevState = currState;
  }

  // Room3 switch



}

void SeeSawPosition(int val){
  if (val < 600){
    currStateSS = "DIP RIGHT";
  } else if (val < 750){
    currStateSS = "MIDDLE";
  } else{
    currStateSS = "DIP LEFT";
  }
  delay (100);

  if (prevStateSS != currStateSS){
    Serial.println(currStateSS);
    prevStateSS = currStateSS;
  }
}

// opens door for servo s taking x seconds, with set speed
void OpenDoor(int amt, Servo &s){
  s.write(80);
  delay(amt);
  s.write(90);
}
// closes door for servo s taking x seconds, with set speed
void CloseDoor(int amt, Servo &s){
  s.write(100);
  delay(amt);
  s.write(90);
}
