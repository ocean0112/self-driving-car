#include "Arduino.h"
#pragma once
// class Ultrasonic 
class Ultrasonic {
private:
  byte triggerPin;
  byte echoPin;
                                  //ft * in * tm * 2-way
  const unsigned long maxDuration = 11 * 12 * 74 * 2; 
  
public:
  Ultrasonic();
  Ultrasonic(byte signal);
  Ultrasonic(byte trigger, byte echo);
  unsigned long measure();
  double measureIN();
};

Ultrasonic::Ultrasonic() : triggerPin{0}, echoPin{0} {}

Ultrasonic::Ultrasonic(byte signal)
   : triggerPin{signal}, echoPin{signal} {}

Ultrasonic::Ultrasonic(byte trigger, byte echo)
   : triggerPin{trigger}, echoPin{echo} {}


unsigned long Ultrasonic::measure() {
  unsigned long duration = 0;
  
  // disable interrupts
  noInterrupts();
  
  // setup  
  pinMode(echoPin, OUTPUT);
  //pinMode(echoPin, INPUT);
  digitalWrite(echoPin, LOW);
  
  // trigger a measurement
  pinMode(triggerPin, OUTPUT);
  digitalWrite(triggerPin, LOW);
  delayMicroseconds(5);
  digitalWrite(triggerPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(triggerPin, LOW);
  
  // wait for 
  pinMode(echoPin, INPUT);
  duration = pulseIn(echoPin, HIGH,  maxDuration);  

  // turn interrupts back on
  interrupts();
  
  // get 1-way duration
  if (duration)
    duration /= 2;
  
  return duration;
} // measure

double Ultrasonic::measureIN() { 
  double inches = 0;
  
  unsigned long duration = measure();
  if (duration){
    inches = duration / 74.0;
  }
    
  if (inches == 0){ 
    //ultrasonic wouldn't catch the incoming waves, so just set it to 40 which is the max distance. 
    inches = 40;
  }
  
  return inches;
} // measureIN
 // end class Ultrasonic
