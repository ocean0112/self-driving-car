#include <Servo.h>
#include <IRremote.h>
#include "motor.h"
#include "ultrasonic.h"
#define IR_USE_AVR_TIMER1

// servo pins
const byte SIGNAL_PIN = A2;
// remote pins
const byte IR_RECEIVE_PIN = 12;
bool tmMeasure = false;
double distance = -1.0;
/*subject to change*/ const int TIMER1_VAL = -(2500000 / (1 / (16/1024)));

// Motor Objects
Motor leftMotor;
Motor rightMotor;

// ultrasonic servo Object
Ultrasonic us(A1, A0);
Servo spin;

// enum Movement
enum Movement{
  turnLeft,
  turnRight,
  goFoward,
  scan,
  stop
}; //end enum Movement
int speed = 100;
void setup()
{
  // timers
  //setupTimer1();
  // timers end
    Serial.begin(9600);
    
    // sets up the servo
    spin.attach(SIGNAL_PIN,500, 2500);  
    
    // sets left and right wheel
    leftMotor = Motor(H1A, H2A, H12EN);
    rightMotor = Motor(H3A, H4A, H34EN);
    
    // sets up ir remote
    IrReceiver.begin(IR_RECEIVE_PIN, true);
    
    // sets speed once
    rightMotor.setSpeed(speed);
    leftMotor.setSpeed(speed);
   // interrupts();
}

const int DELAY = 5 * 1000;
double measurefront = 8;
double measureleft = 8;
double measureright = 80;
double frontleft = 0;
double frontright = 0;
//double wall = 7;
byte wall = 20;

void loop(){
  if (!IrReceiver.decode()) { //IR sensor
    switch(IrReceiver.decodedIRData.command){
      case btn_2:
        Serial.println(" used remote ... foward...");
        leftMotor.run(Motor::MotorFoward);
        rightMotor.run(Motor::MotorFoward);
        break;
      case btn_4:
        Serial.println("used remote ... left...");
          leftMotor.run(Motor::MotorReverse);
          rightMotor.run(Motor::MotorFoward);
          break;
      case btn_6:
        Serial.println("used remote ... right...");
        leftMotor.run(Motor::MotorFoward);
        rightMotor.run(Motor::MotorReverse);
        break;
      default:
        Serial.println("used remote ... stop...");
        leftMotor.run(Motor::MotorStop);
        rightMotor.run(Motor::MotorStop);
    }
  }
  if (tmMeasure) {
    // stop and scans
    
    Serial.println("stop...");
    leftMotor.run(Motor::MotorStop);
    rightMotor.run(Motor::MotorStop);
    
    // perform measures on each direction (left, forward, right)
    //measureleft
    spin.write(180);
    delay(1300);
    measureleft = us.measureIN();
    Serial.println(measureleft);
    delay(100);
    //measure the 135 angle
    spin.write(135);
    delay(1300);
    frontleft = us.measureIN();
    Serial.println(frontleft);
    delay(100);
    //measures the front
    spin.write(90);
    delay(1300);
    measurefront = us.measureIN();
    Serial.println(measurefront);
    delay(100);
    //measures the 45 angle
    spin.write(45);
    delay(1300);
    frontright = us.measureIN();
    Serial.println(frontright);
    delay(100);
    //measures the right
    spin.write(0);
    delay(1300);
    measureright = us.measureIN();
    Serial.println(measureright);
    delay(1300);


    if(measurefront >= 30 && measureright >= 30 && measureleft >= 30 && frontleft >= 30 && frontright >= 30){
      // final condition check
      Serial.println("foward...");
      leftMotor.run(Motor::MotorFoward);
      rightMotor.run(Motor::MotorFoward);
      delay(100);
      leftMotor.run(Motor::MotorStop);
      rightMotor.run(Motor::MotorStop);
      spin.write(90);
      delay(1300);
      double measurefinalfront = us.measureIN();
      Serial.println(measurefinalfront);
      delay(1300);
      spin.write(90);
      delay(1300);
      double measurefinalleft = us.measureIN();
      Serial.println(measurefinalleft);
      delay(1300);
      spin.write(90);
      delay(1300);
      double measurefinalright = us.measureIN();
      Serial.println(measurefinalright);
      delay(1300);
      if (measurefinalfront > 30 && measurefinalleft > 30 && measurefinalright > 30){ //outside of maze
        Serial.print("out of maze");
        leftMotor.run(Motor::MotorStop);
        rightMotor.run(Motor::MotorStop);
        delay(1000000);
      }
      else{
        
      }
    }
    //if the front is against the wall
    else if (measurefront < 2){
      Serial.println("reverse");
      leftMotor.setSpeed(10);
      rightMotor.setSpeed(10);
      rightMotor.run(Motor::MotorReverse);
      leftMotor.run(Motor::MotorReverse);
      delay(100);
      leftMotor.run(Motor::MotorStop);
      rightMotor.run(Motor::MotorStop);
      if (measureright > measureleft){
        Serial.println("right...");
        leftMotor.setSpeed(10);
        rightMotor.setSpeed(10);
        rightMotor.run(Motor::MotorReverse);
        leftMotor.run(Motor::MotorFoward);
        delay(225);
         leftMotor.run(Motor::MotorStop);
        rightMotor.run(Motor::MotorStop);
      }
      else{
        Serial.println("left...");
        leftMotor.setSpeed(10);
        rightMotor.setSpeed(10);
        rightMotor.run(Motor::MotorFoward);
        leftMotor.run(Motor::MotorReverse);
        delay(225);
        leftMotor.run(Motor::MotorStop);
        rightMotor.run(Motor::MotorStop);

      }
    }
    else if(measurefront > measureleft && measurefront > measureright){ //if the front is clear
      if (measureleft < 6 && measureright > measureleft){
      //if (measureright > measureleft){
        Serial.println("right...");
        leftMotor.setSpeed(10);
        rightMotor.setSpeed(10);
        rightMotor.run(Motor::MotorReverse);
        leftMotor.run(Motor::MotorFoward);
        delay(125);
         leftMotor.run(Motor::MotorStop);
        rightMotor.run(Motor::MotorStop);
      }
      if (frontleft < 6 && measureright > measureleft){
      //if (measureright > measureleft){
        Serial.println("right...");
        leftMotor.setSpeed(10);
        rightMotor.setSpeed(10);
        rightMotor.run(Motor::MotorReverse);
        leftMotor.run(Motor::MotorFoward);
        delay(125);
         leftMotor.run(Motor::MotorStop);
        rightMotor.run(Motor::MotorStop);
      }
      else if (measureright < 6 && measureleft > measureright){
        Serial.println("left...");
        leftMotor.setSpeed(10);
        rightMotor.setSpeed(10);
        rightMotor.run(Motor::MotorFoward);
        leftMotor.run(Motor::MotorReverse);
        delay(125);
        leftMotor.run(Motor::MotorStop);
        rightMotor.run(Motor::MotorStop);
      }
      else if (frontright < 6 && measureleft > measureright){
        Serial.println("left...");
        leftMotor.setSpeed(10);
        rightMotor.setSpeed(10);
        rightMotor.run(Motor::MotorFoward);
        leftMotor.run(Motor::MotorReverse);
        delay(125);
        leftMotor.run(Motor::MotorStop);
        rightMotor.run(Motor::MotorStop);
      }else{
        Serial.print("else");

      }
    } // end of front functions
    else if(measureleft > measureright){

      Serial.println("left...");
      leftMotor.setSpeed(10);
      rightMotor.setSpeed(10);
      rightMotor.run(Motor::MotorFoward);
      leftMotor.run(Motor::MotorReverse);
      delay(125);
      leftMotor.run(Motor::MotorStop);
      rightMotor.run(Motor::MotorStop);
    }// end of left functions
    else if(measureright > measureleft){
      Serial.println("right...");
      leftMotor.setSpeed(10);
      rightMotor.setSpeed(10);
      rightMotor.run(Motor::MotorReverse);
      leftMotor.run(Motor::MotorFoward);
      delay(125);
      leftMotor.run(Motor::MotorStop);
      rightMotor.run(Motor::MotorStop);
      }// right fucntions
      tmMeasure = false;
    }//end of else of main function
 // }
  else {
    delay(2000);
    Serial.println("foward...");
    leftMotor.run(Motor::MotorFoward);
    rightMotor.run(Motor::MotorFoward);
    delay(500);
    tmMeasure = true;
  }
}
/*
// subjected to change
ISR(TIMER1_OVF_vect) {
  tmMeasure = true;
  //TCNT1 = TIMER1_VAL;
  TCNT1H = (TIMER1_VAL >> 8);
  TCNT1L = (TIMER1_VAL & 0x00FF);
}

void setupTimer1() { 
            // des     s   XTAL / scaler
  //TCNT1 = TIMER1_VAL;
  TCNT1H = (TIMER1_VAL >> 8);
  TCNT1L = (TIMER1_VAL & 0x00FF);
  TCCR1A = 0;    // normal mode
  TCCR1B = 0x05; // 1024 prescaler
  TIMSK1 = (1<<TOIE1);
}
*/
