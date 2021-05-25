#include "Arduino.h"
#include "GripSensor.h"
#include "RGBLed.h"

GripSensor::GripSensor(int low_position_pin, int high_position_pin) {
  //constructor
  _low_position_pin = low_position_pin;
  _high_position_pin = high_position_pin;
  _debug = false;
  _good_relay = false;
}
void GripSensor::init() {
  pinMode(_low_position_pin, INPUT);
  pinMode(_high_position_pin, INPUT);
}
void GripSensor::_read_grip(){
  _low_position_change = digitalRead(_low_position_pin);
  _high_position_change = digitalRead(_high_position_pin);
}
void GripSensor::read_grip(){
  _low_position_grip = digitalRead(_low_position_pin);
  _high_position_grip = digitalRead(_high_position_pin);
}

void GripSensor::print_status() {
  //Serial.print("Transition zone = "); Serial.println(transition_zone);
  Serial.print("low_position_grip  = "); Serial.println(_low_position_grip);
  Serial.print("high_position_grip  = "); Serial.println(_high_position_grip);
}

bool GripSensor::high_grip() {
  return _high_position_grip;
}

bool GripSensor::low_grip() {
  return _low_position_grip;
}

bool GripSensor::grip_ok() {
  //if one of sensors is griped it works, if not one
  //returns false
  return _low_position_grip && _high_position_grip ||
         (!_low_position_grip && !_high_position_grip);
}
void GripSensor::enable_debug() {
  _debug = true;
}
bool GripSensor::good_relay(){
  if(_low_position_grip && !_good_relay){
    _read_grip();
    if(_high_position_change && !_low_position_change)
      _good_relay = true;
    else
      _good_relay = false;
  }
  if(_high_position_grip && !_good_relay){
    _read_grip();
    if(!_high_position_change && _low_position_change)
      _good_relay = true;
    else
      _good_relay = false;
  }
  return _good_relay;
}

void GripSensor::reset_relay(RGBLed led_rgb) {
  if (!_good_relay)
    led_rgb.blink_red(4);
  _good_relay = false;
}
