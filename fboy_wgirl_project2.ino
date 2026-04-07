// imports
#include <Servo.h>

////// VARS //////
// Servos
Servo servo1_360;
Servo servo2_360;
Servo servo3_180;
Servo servo4_180;
int s1pin;
int s2pin = 12;
int s3pin = 3;
int s4pin;
// LEDs
int rgbled_Rpin = 11;
int rgbled_Gpin = 9;
int rgbled_Bpin = 10;
// Copper Tape Switches
int switch1pin;
int switch2pin = A1;
int switch3pin = A2;
int switch4pin = A3;




// State vars
int prevStateR2 = LOW;
int currStateR2 = LOW;
int prevStateR3_left = LOW;
int currStateR3_left = LOW;
int prevStateR3_right = LOW;
int currStateR3_right = LOW;
String currPositionSeeSaw = "LEFT";




void setup() {
  ////// INITALIZE CONNECTIONS //////
  Serial.begin(9600);
  // Servos
  servo1_360.attach(s1pin);
  servo2_360.attach(s2pin);
    servo2_360.write(90);
  servo3_180.attach(s3pin);
    servo3_180.write(85);
  servo4_180.attach(s4pin);
  delay(500);
  // LEDs
  pinMode(rgbled_Rpin, OUTPUT);
  pinMode(rgbled_Gpin, OUTPUT);
  pinMode(rgbled_Bpin, OUTPUT);
  // Copper Tape Switches
  pinMode(switch2pin, INPUT);

  //OpenDoor(800, servo2_360);

  // middle is 40, LEFT is 85, RIGHT is 0
  //servo3_180.write(40);

}

void loop() {
  // Room2 switches
  currStateR2 = digitalRead(switch2pin);
  if (prevStateR2 != currStateR2){
    if (currStateR2 == HIGH){
      OpenDoor(2000, servo2_360);
      delay(2000);
    } else{
      CloseDoor(2000, servo2_360);
      delay(2000);
    }
    prevStateR2 = currStateR2;
  }

  // Room3 switches
  currStateR3_left = digitalRead(switch3pin);
  currStateR3_right = digitalRead(switch4pin);
  if ((prevStateR3_left != currStateR3_left) || (prevStateR3_right != currStateR3_right)){
    // left up, right down --> dip right
    if (currStateR3_left == LOW && currStateR3_right == HIGH){
      Serial.println("TILT RIGHT");
      SeeSawDip("RIGHT", currPositionSeeSaw, servo3_180);
      currPositionSeeSaw = "RIGHT";
    // left down, right up --> dip left
    } else if (currStateR3_left == HIGH && currStateR3_right == LOW){
      Serial.println("TILT LEFT");
      SeeSawDip("LEFT", currPositionSeeSaw, servo3_180);
      currPositionSeeSaw = "LEFT";
    }
    // all other switch scenarios do not move the seesaw
    delay(850);
    prevStateR3_left = currStateR3_left;
    prevStateR3_right = currStateR3_right;
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

// gradually dips the seesaw using servo s in the desired direction, if changed
void SeeSawDip(String dir, String currPos, Servo &s){
  // note: middle is 40, LEFT is 85, RIGHT is 0
  if (dir == "LEFT"){
    Serial.println("We are going to dip down left, from the current position "+currPos);
    if (dir != currPos){
      int pos = 0;
      while (pos < 85){
        s.write(pos);
        pos += 1;
        delay(10);
      }
    }
  } else if (dir == "RIGHT"){
    Serial.println("We are going to dip down right, from the current position "+currPos);
    if (dir != currPos){
      int pos = 85;
      while (pos > 0){
        s.write(pos);
        pos -= 1;
        delay(10);
      }
    }
  } else {
    Serial.println("SeeSawDip() direction string invalid");
  }

}
