#include "Arduino.h"
#include "GripSensor.h"
#include "RGBLed.h"
/// \brief Contructeur
/// \param low_position_pin Le pin où se trouve la signal du capteur touch inferieur
/// \param high_position_pin Le pin où se trouve la signal du capteur touch supeireur
GripSensor::GripSensor(int low_position_pin, int high_position_pin) {
  //constructor
  _low_position_pin = low_position_pin;
  _high_position_pin = high_position_pin;
  _debug = false;
  _good_relay = false;
}
/// \brief Cette methode initialise les pins pour les capteur touch du temoin
void GripSensor::init() {
  pinMode(_low_position_pin, INPUT);
  pinMode(_high_position_pin, INPUT);
}
void GripSensor::_read_grip(){
  _low_position_change = digitalRead(_low_position_pin);
  _high_position_change = digitalRead(_high_position_pin);
}
/// \brief Cette methode lit les entrées des capteur touch
void GripSensor::read_grip(){
  _low_position_grip = digitalRead(_low_position_pin);
  _high_position_grip = digitalRead(_high_position_pin);
}
/// \brief Cette methode montre les signal des deux capteurs touch
void GripSensor::print_status() {
  //Serial.print("Transition zone = "); Serial.println(transition_zone);
  Serial.print("low_position_grip  = "); Serial.println(_low_position_grip);
  Serial.print("high_position_grip  = "); Serial.println(_high_position_grip);
}
/// \brief Cette methode montre la valeur du capteur superieur
bool GripSensor::high_grip() {
  return _high_position_grip;
}
/// \brief Cette methode montre la valeur du capteur inferieur
bool GripSensor::low_grip() {
  return _low_position_grip;
}
/// \brief Cette methode nous dit si le temoin est pris par une des cotés du temoin, pas les deux cotés (Basiquement fait un fonction XOR)
bool GripSensor::grip_ok() {
  //if one of sensors is griped it works, if not one
  //returns false
  return _low_position_grip && _high_position_grip ||
         (!_low_position_grip && !_high_position_grip);
}
/// \brief Cette methode teste si l'athlete a fait un bon relay.
/// \return TRUE Si les grip des capteur touch changes dans la zone de transmission
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
