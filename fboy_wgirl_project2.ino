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
int s4pin = 7;
// LEDs
int beaconLED_pin = 5;
int rgbled_Rpin;
int rgbled_Gpin;
int rgbled_Bpin;
// Copper Tape Switches
int switch1pin;
int switch2pin = A1;
int switch3pin = A2;
int switch4pin = A3;
int switch5pin = A4;
int switch6pin = A5;
// State vars
int prevStateR2 = LOW;
int currStateR2 = LOW;
int prevStateR3_left = LOW;
int currStateR3_left = LOW;
int prevStateR3_right = LOW;
int currStateR3_right = LOW;
String currPositionSeeSaw = "LEFT";
int prevStateR4 = LOW;
int currStateR4 = LOW;
bool iceTrapActivated;
bool iceMelted;
int currStateLever = LOW;
int prevStateLever = LOW;


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
    servo4_180.write(90);
  delay(1000);
  // LEDs
  pinMode(beaconLED_pin, OUTPUT);
  //pinMode(rgbled_Rpin, OUTPUT);
  //pinMode(rgbled_Gpin, OUTPUT);
  //pinMode(rgbled_Bpin, OUTPUT);
  // Copper Tape Switches
  pinMode(switch2pin, INPUT);
  pinMode(switch3pin, INPUT);
  pinMode(switch4pin, INPUT);
  pinMode(switch5pin, INPUT);
  pinMode(switch6pin, INPUT);
  // Other
  iceTrapActivated = false;
  iceMelted = false;
  digitalWrite(beaconLED_pin, LOW);
  // TESTING //

  //CloseDoor(800, servo2_360);


}


void loop() {
  // Room2 switches - door
  currStateR2 = digitalRead(switch2pin);
  if (prevStateR2 != currStateR2){
    if (currStateR2 == HIGH){
      OpenDoor(2000, servo2_360);
    } else{
      CloseDoor(2000, servo2_360);
    }
    delay(2000);
    prevStateR2 = currStateR2;
  }

  // Room3 switches - seesaw
  currStateR3_left = digitalRead(switch3pin);
  currStateR3_right = digitalRead(switch4pin);
  if ((prevStateR3_left != currStateR3_left) || (prevStateR3_right != currStateR3_right)){
    // left up, right down --> dip right
    if (currStateR3_left == LOW && currStateR3_right == HIGH){
      SeeSawDip("RIGHT", currPositionSeeSaw, servo3_180);
      currPositionSeeSaw = "RIGHT";
    // left down, right up --> dip left
    } else if (currStateR3_left == HIGH && currStateR3_right == LOW){
      SeeSawDip("LEFT", currPositionSeeSaw, servo3_180);
      currPositionSeeSaw = "LEFT";
    }
    // all other switch scenarios do not move the seesaw
    delay(850);
    prevStateR3_left = currStateR3_left;
    prevStateR3_right = currStateR3_right;
  }

  // Room4 switches - ice
  // ice patch
  currStateR4 = digitalRead(switch5pin);
  if (prevStateR4 != currStateR4 && !iceTrapActivated){
    if (currStateR4 == HIGH){
      gradual_motion_180servo(2000, servo4_180, 100, 0);
      iceTrapActivated = true;
      delay(2000);
    }
    prevStateR4 = currStateR4;
  }
  // lever
  currStateLever = digitalRead(switch6pin);
  if (currStateLever != prevStateLever){
    if (currStateLever == HIGH){
      digitalWrite(beaconLED_pin, HIGH);
      // lever turned on AND ice trap is up --> melt away ice
      if (iceTrapActivated && !iceMelted){
        gradual_motion_180servo(2000, servo4_180, 0, 100);
        iceMelted = true;
        delay(3000);
      }
    } else{
      digitalWrite(beaconLED_pin, LOW);
    }
    prevStateLever = currStateLever;
    delay(1000);
  }

}

////// CUSTOM FUNCTIONS //////

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
// note: MIDDLE is 40, LEFT is 85, RIGHT is 0
void SeeSawDip(String dir, String currPos, Servo &s){
  // request to dip left AND not already in left position --> dip left
  if (dir == "LEFT"){
    if (dir != currPos){
      int pos = 0;
      while (pos < 85){
        s.write(pos);
        pos += 1;
        delay(10);
      }
    }
  // request to dip right AND not already in right position --> dip right
  } else if (dir == "RIGHT"){
    if (dir != currPos){
      int pos = 85;
      while (pos > 0){
        s.write(pos);
        pos -= 1;
        delay(10);
      }
    }
  // error handling
  } else {
    Serial.println("SeeSawDip() direction string invalid");
  }
}

// gradually moves a 180 servo s from startAngle -> destAngle in x seconds
void gradual_motion_180servo(float time, Servo &s, int startAngle, int destAngle){
  // high --> low (eg. 180 --> 90)
  if (startAngle > destAngle){
    while (startAngle > destAngle){
      s.write(startAngle);
      startAngle -= 1;
      delay(10);
    }
  // low --> high (eg. 90 --> 180)
  } else{
    while (startAngle < destAngle){
      s.write(startAngle);
      startAngle += 1;
      delay(10);
    }
  }
}
