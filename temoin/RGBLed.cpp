#include "Arduino.h"
#include "RGBLed.h"
#include "Buzzer.h"

RGBLed::RGBLed(int pinRed, int pinGreen, int pinBlue) {
  //constructor
  _pinRed = pinRed;
  _pinGreen = pinGreen;
  _pinBlue = pinBlue;
  _buzz_state = 0;
}
RGBLed::RGBLed(int pinRed, int pinGreen, int pinBlue, int buzz) {
  //constructor
  _pinRed = pinRed;
  _pinGreen = pinGreen;
  _pinBlue = pinBlue;
  _buzz_state = 1;
  _buzz = buzz;
}
void RGBLed::init() {
  pinMode(_pinRed, OUTPUT);
  pinMode(_pinGreen, OUTPUT);
  pinMode(_pinBlue, OUTPUT);
}
void RGBLed::lights_on() {
  analogWrite(_pinRed, _red);
  analogWrite(_pinGreen, _green);
  analogWrite(_pinBlue, _blue);
}
void RGBLed::set_color(int red, int green, int blue) {
  _red = red;
  _green = green;
  _blue = blue;
  analogWrite(_pinRed, red);
  analogWrite(_pinGreen, green);
  analogWrite(_pinBlue, blue);
  //lights_on();
}
void RGBLed::set_blue() {
  set_color(0, 0, 245);
}
void RGBLed::set_red() {
  set_color(254, 0, 0);
}
void RGBLed::set_green() {
  set_color(0, 254, 0);
}
void RGBLed::set_yellow() {
  set_color(254, 254, 0);
}
void RGBLed::blink_red(int times) {
  int delay_ = 500;
  for (int i = 0; i < times; i++)
  {
    if(_buzz_state) analogWrite(_buzz, 255);
    set_red();
    delay(delay_);
    if(_buzz_state) analogWrite(_buzz, 0);
    set_blue();
    delay(delay_);
  }
}
void RGBLed::blink_green(int times) {
  int delay_ = 200;
  for (int i = 0; i < times; i++)
  {
    //if(_buzz_state) analogWrite(_buzz, 255);
    set_green();
    delay(delay_);
    //if(_buzz_state) analogWrite(_buzz, 0);
    set_blue();
    delay(delay_);
  }
}
void RGBLed::test() {
  //Serial.print("Pines ");
  //Serial.print("R =");Serial.print(_pinRed);
  //Serial.print("G = ");Serial.print(_pinGreen);
  //Serial.print("B = ");Serial.print(_pinBlue);
  int delay_ = 500;
  if (_buzz_state) {
    set_yellow();
    analogWrite(_buzz, 88);
    delay(delay_);
    set_blue();
    analogWrite(_buzz, 150);
    delay(delay_);
    set_red();
    analogWrite(_buzz, 200);
    delay(delay_);
    set_green();
    analogWrite(_buzz, 250);
    delay(delay_);
    digitalWrite(_buzz, 0);
  } else {
    set_yellow();
    delay(delay_);
    set_blue();
    delay(delay_);
    set_red();
    delay(delay_);
    set_green();
    delay(delay_);
  }
}
