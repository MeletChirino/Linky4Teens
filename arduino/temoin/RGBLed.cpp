#include "Arduino.h"
#include "RGBLed.h"
#include "Buzzer.h"
/// \brief Constructeur
/// \param pinRed Pin de la couleur rouge du LED RGB
/// \param pinGreen Pin de la couleur vert du LED RGB
/// \param pinBlue Pin de la couleur blue du LED RGB
RGBLed::RGBLed(int pinRed, int pinGreen, int pinBlue) {
  //constructor
  _pinRed = pinRed;
  _pinGreen = pinGreen;
  _pinBlue = pinBlue;
  _buzz_state = 0;
}

/// \brief Constructeur utilisé quand il y a une Buzzer attahcé au circuit
/// \param pinRed Pin de la couleur rouge du LED RGB
/// \param pinGreen Pin de la couleur vert du LED RGB
/// \param pinBlue Pin de la couleur blue du LED RGB
/// \param buzz Pin du buzzer attaché au circuit
RGBLed::RGBLed(int pinRed, int pinGreen, int pinBlue, int buzz) {
  //constructor
  _pinRed = pinRed;
  _pinGreen = pinGreen;
  _pinBlue = pinBlue;
  _buzz_state = 1;
  _buzz = buzz;
}

/// \brief Cette fonction initalise les pins pour la led RGB
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

/// \brief Ce fonction recoit des valeurs entre 0 et 255.
/// \param red mets les couleur du red
/// \param green mets les couleur du green
/// \param blue mets les couleur du blue
void RGBLed::set_color(int red, int green, int blue) {
  _red = red;
  _green = green;
  _blue = blue;
  analogWrite(_pinRed, red);
  analogWrite(_pinGreen, green);
  analogWrite(_pinBlue, blue);
  //lights_on();
}

/// \brief Ce fonction mets la led RGB en blue
void RGBLed::set_blue() {
  set_color(0, 0, 245);
}

/// \brief Ce fonction mets la led RGB en red
void RGBLed::set_red() {
  set_color(254, 0, 0);
}

/// \brief Ce fonction mets la led RGB en vert
void RGBLed::set_green() {
  set_color(0, 254, 0);
}

/// \brief Ce fonction mets la led RGB en jaune
void RGBLed::set_yellow() {
  set_color(254, 254, 0);
}
/// \brief Ce fonction mets la led RGB en rouge en clignote quelques fois
/// \param times Combien de fois clignotte la LED en rouge
void RGBLed::blink_red(int times) {
  int delay_ = 500;
  for (int i = 0; i < times; i++)
  {
    if (_buzz_state) analogWrite(_buzz, 255);
    set_red();
    delay(delay_);
    if (_buzz_state) analogWrite(_buzz, 0);
    set_blue();
    delay(delay_);
  }
}
/// \brief Ce fonction mets la led RGB en vert en clignote quelques fois
/// \param times Combien de fois clignotte la LED en vert
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
/// \brief Ce fonction teste la LED RGB en ordre rouge, vert, blue, jaune. Utile pour savoir si la LED est bien cablée.
void RGBLed::test() {
  int delay_ = 500;
  if (_buzz_state) {
    set_red();
      analogWrite(_buzz, 88);
    delay(delay_);
    set_green();
    analogWrite(_buzz, 150);
    delay(delay_);
    set_blue();
    analogWrite(_buzz, 200);
    delay(delay_);
    set_yellow();
    analogWrite(_buzz, 250);
    delay(delay_);
    digitalWrite(_buzz, 0);
  } else {
    set_red();
    delay(delay_);
    set_green();
    delay(delay_);
    set_blue();
    delay(delay_);
    set_yellow();
    delay(delay_);
  }
}
