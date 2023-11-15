#include <Servo.h>
#include "motor.h"
#include "ultrasonic.h"
// servo const
const byte SIGNAL_PIN;

/*subject to change*/ const int TIMER1_VAL = -(250000 / (1 / (16/1024)));
// Motor variables
Motor leftMotor;
Motor rightMotor;

// enum Movement
enum Movement{
  turnLeft,
  turnRight,
  goFoward,
  scan,
  stop
}; //end enum Movement

void setup()
{
    Serial.begin(9600);
    leftMotor = Motor(H1A, H2A, H12EN);
    rightMotor = Motor(H3A, H4A, H34EN);
}

byte speed = 0;
const int DELAY = 2 * 1000;

void loop()
{
    Serial.println("foward...");
    leftMotor.run(Motor::MotorFoward);
    rightMotor.run(Motor::MotorFoward);
    delay(DELAY);

    Serial.println("left...");
    leftMotor.run(Motor::MotorReverse);
    rightMotor.run(Motor::MotorFoward);
    delay(DELAY);

    Serial.println("foward...");
    leftMotor.run(Motor::MotorReverse);
    rightMotor.run(Motor::MotorReverse);
    delay(DELAY);

    Serial.println("right...");
    leftMotor.run(Motor::MotorFoward);
    rightMotor.run(Motor::MotorReverse);
    delay(DELAY);

    Serial.println("stop...");
    leftMotor.run(Motor::MotorStop);
    rightMotor.run(Motor::MotorStop);
    delay(DELAY);

    speed += 15;
    Serial.print("speed: ");
    Serial.println(speed);
    rightMotor.setSpeed(speed);
    leftMotor.setSpeed(speed);
}

// subjected to change
ISR(TIMER1_OVF_vect) {
  tmMeasure = true;
//  TCNT1 = TIMER1_VAL;
  TCNT1H = (TIMER1_VAL >> 8);
  TCNT1L = (TIMER1_VAL & 0x00FF);
}

void setupTimer1() { 
            // des     s   XTAL / scaler
//  TCNT1 = TIMER1_VAL;
  TCNT1H = (TIMER1_VAL >> 8);
  TCNT1L = (TIMER1_VAL & 0x00FF);
  TCCR1A = 0;    // normal mode
  TCCR1B = 0x05; // 1024 prescaler
  TIMSK1 = (1<<TOIE1);
}

