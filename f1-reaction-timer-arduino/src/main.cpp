#include <Arduino.h>

const int lights[]={13,12,11,10,9};
const int n=5;

const int dt=1000;
const int dmin=200;
const int dmax=3000;
const int dreset=2000;

const int noise=A0;

const int button=7;

unsigned long stepStart;
unsigned long lightsOut;

int step=0;
const int PRESS=n+1;
const int PAUSE=n+2;

void allLightsOut(){
  for(int i=0;i<n;i++) digitalWrite(lights[i],LOW);
}

bool buttonPressed(){return digitalRead(button)==LOW;}

void showReactionTime(unsigned long ms){
  Serial.print("reaction time: ");
  Serial.print(ms);
  Serial.println(" ms");
}

void showJumpStart(){
  Serial.println("---jump start!---");
}

void setup() {
  Serial.begin(9600);
  for(int i=0;i<n;i++) pinMode(lights[i],OUTPUT);
  randomSeed(analogRead(noise));
  pinMode(button,INPUT_PULLUP);
}

void loop() {
  if(step<n && millis()-stepStart >= dt){ //5 lights
    digitalWrite(lights[step],HIGH);
    step++;
    stepStart=millis();
    if(step==n){
      lightsOut=random(dmin,dmax); //only after last light
    }
  }
  if(step==n && millis()-stepStart >= lightsOut){
    allLightsOut();
    step=PRESS;
    stepStart=millis();
  }

  if(buttonPressed()){
    if(step<=n){        //jump start
      allLightsOut();
      showJumpStart();
      step=PAUSE;
      stepStart=millis();
    }
    else if(step==PRESS){  //reaction time
      unsigned long reactionTime=millis()-stepStart;
      showReactionTime(reactionTime);
      step=PAUSE;
      stepStart=millis();
    }
  }

  if(step==PAUSE && millis()-stepStart >= dreset && !buttonPressed()){
    step=0;
    stepStart=millis();
  }
}