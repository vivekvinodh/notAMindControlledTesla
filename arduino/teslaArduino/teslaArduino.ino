#include "TimerOne.h"
#define PULSE 300

const int outPin = 10;
const int RETRACT = 1300;
const int EXTEND = 1100;
const int ERR = 1700;
const int RETRACTGAS = 1700;
const int FORCEBRAKE = 900;

int channels[6] = {1200, 1200, 1200, 1200, 1200, 1200};
//channels[0] = CH1/BRAKE - RELIABLE 1200-FLAT (x>1300)-RETRACT (x<1100)-EXTEND 
//channels[1] = CH2/GAS - RELIABLE 1200-FLAT (x>1300)-RETRACT (x<1100)-EXTEND
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
    channels[index] = i;
  }
}

void scaleDown(int index, int origin, int destination){
  for(int i = origin; i >= destination; i--){
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
      scaleUp(1,channels[1],RETRACT);
      scaleDown(0,channels[0],EXTEND-100);
      //channels[1] = DOWN;
      //channels[0] = UP;
    }
    else {
      if(atoi(this_line.c_str()) == 1){
        //update channel
        Serial.println("go");
        scaleDown(1, channels[1], EXTEND);
        scaleUp(0, channels[0], RETRACT+100);
        //channels[1] = UP;
        //channels[0] = DOWN;
      }
      else{
        //assume crit fail, reset actuators
        Serial.println("err");
        channels[3] = ERR;
        channels[0] = FORCEBRAKE;
        channels[1] = RETRACTGAS;
      }
    }
  }
}

