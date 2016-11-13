#include "TimerOne.h"
#define PULSE 300

const int outPin = 10;

int channels[6] = {1200, 1200, 1200, 1200, 1200, 1200};
//channels[0] = CH1 - RELIABLE 1200-FLAT 1300-DOWN 1100-UP
//channels[1] = CH2 - RELIABLE 1200-FLAT 1300-DOWN 1100-UP
//channels[2] = CH3
//channels[3] = CH4
//channels[4] = CH5
//channels[5] = CH6

byte inputCommand; // 0 == stop; 1 == go
String this_line;
void setup() {
  // put your setup code here, to run once:
  pinMode(outPin, OUTPUT);
  digitalWrite(outPin, HIGH);
  Timer1.initialize(18000);
  Timer1.attachInterrupt(relay);
  Serial.begin(57600);
  delay(500);
  Serial.println("I'm on!");
}

void pulse() {
  digitalWrite(outPin, LOW);
  delayMicroseconds(PULSE);
  digitalWrite(outPin, HIGH);
}

void channel(int delay){
  pulse();
  delayMicroseconds(delay);
}

void loop() {
  // put your main code here, to run repeatedly:
  readBuf();
}

void relay(void){
  for(int i = 0; i < 6; i++){
    channel(channels[i]);
  }
}

void scaleUp(int index, int origin, int destination){
  for(int i = origin; i <= destination; i++){
    delay(1);
    channels[index] = i;
  }
}

void scaleDown(int index, int origin, int destination){
  for(int i = origin; i >= destination; i--){
    delay(1);
    channels[index] = i;
  }
}

void readBuf(){
  if(Serial.available()){
    while(Serial.available() == 0){
        //wait for new char
    }
    inputCommand = Serial.read();
    if(inputCommand == 0x00){
      Serial.println("stop");
      //update channel
      scaleUp(1,channels[1],1300);
      scaleDown(0,channels[0],1100);
      //channels[1] = 1300;
      //channels[0] = 1100;
    }
    else {
      if(inputCommand == 0x01){
        Serial.println("go");
        //update channel
        scaleDown(1, channels[1], 1100);
        scaleUp(0, channels[0], 1300);
        //channels[1] = 1100;
        //channels[0] = 1300;
      }
      else{
        Serial.println("ERROR");
        //assume crit fail, reset actuators
        channels[0] = 1200;
        channels[1] = 1200; 
      }
    }
  }
}

