#pragma once
// motor const might need be changed
const int H1A;
const int H2A;
const int H12EN;

const int H3A;
const int H4A;
const int H34EN;
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