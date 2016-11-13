#include "TimerOne.h"
#define PULSE 300

const int outPin = 10;
const int DOWN = 1300;
const int UP = 1100;
const int ERR = 1700;

int channels[6] = {1200, 1200, 1200, 1200, 1200, 1200};
//channels[0] = CH1 - RELIABLE 1200-FLAT 1300-DOWN 1100-UP
//channels[1] = CH2 - RELIABLE 1200-FLAT 1300-DOWN 1100-UP
//channels[2] = CH3
//channels[3] = CH4
//channels[4] = CH5
//channels[5] = CH6

char inputCommand; // 0 == stop; 1 == go
String this_line;
void setup() {
  // put your setup code here, to run once:
  pinMode(outPin, OUTPUT);
  pinMode(LED_BUILTIN, OUTPUT);
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
    //Serial.print(channels[i]);
    channel(channels[i]);
  }
  //Serial.println("--------");
}

void scaleUp(int index, int origin, int destination){
  for(int i = origin; i <= destination; i++){
    //delay(1);
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
    this_line = inputCommand;
    if(atoi(this_line.c_str()) == 0){
      //update channel
      Serial.println("stop");
      scaleUp(1,channels[1],DOWN);
      scaleDown(0,channels[0],UP);
      //channels[1] = DOWN;
      //channels[0] = UP;
    }
    else {
      if(atoi(this_line.c_str()) == 1){
        //update channel
        Serial.println("go");
        scaleDown(1, channels[1], UP);
        scaleUp(0, channels[0], DOWN);
        //channels[1] = UP;
        //channels[0] = DOWN;
      }
      else{
        //assume crit fail, reset actuators
        channels[3] = ERR;
      }
    }
  }
}

