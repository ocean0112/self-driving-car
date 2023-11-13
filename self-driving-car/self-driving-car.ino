#include <Servo.h>
// motor const might need be changed
const int H1A;
const int H2A;
const int H12EN;

const int H3A;
const int H4A;
const int H34EN;
// servo const
const byte SIGNAL_PIN;

/*subject to change*/ const int TIMER1_VAL = -(250000 / (1 / (16/1024)));
// end const

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
  pinMode(echoPin, INPUT);
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
  duration = pulseIn(echoPin, maxDuration);
  
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
  
  if (duration)
    inches = duration / 74.0;
  
  return inches;
} // measureIN
 // end class Ultrasonic 

// class Motor
class Motor {
private:
    byte _pin1;
    byte _pin2;
    byte _enable;
    byte _speed;
public:
    enum MotorDir {
        MotorStop,
        MotorFoward,
        MotorReverse,
    }; //enum MotorDir

    Motor() : _pin1{ 0 }, _pin2{ 0 }, _enable{ 0 }, _speed{ 0 } {}

    Motor(byte pin1, byte pin2, byte enable)
        :_pin1{ pin1 }, _pin2{ pin2 }, _enable{ enable }, _speed{ 0 } {

        pinMode(_pin1, OUTPUT);
        pinMode(_pin2, OUTPUT);
        pinMode(_enable, OUTPUT);
    }

    byte getSpeed() { return _speed; }
    void setSpeed(byte speed) { _speed = speed; }

    void run(Motor::MotorDir dir) {
        switch (dir) {
        case MotorStop:
            digitalWrite(_pin1, LOW);
            digitalWrite(_pin2, LOW);
            digitalWrite(_enable, LOW);
            break;
        case MotorFoward:
            digitalWrite(_pin1, HIGH);
            digitalWrite(_pin2, LOW);
            digitalWrite(_enable, _speed);
            break;
        case MotorReverse:
            digitalWrite(_pin1, LOW);
            digitalWrite(_pin2, HIGH);
            digitalWrite(_enable, _speed);
        } // which dir
    } // end run
}; // end Motor

// Motor variables
Motor leftMotor;
Motor rightMotor;

// enum Movement
enum Movement{
  turnLeft,
  turnRight,
  goFoward
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
    switch(){

    }
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

