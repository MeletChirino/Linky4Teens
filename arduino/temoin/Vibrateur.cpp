#include "Arduino.h"
#include "Vibrateur.h"
/// \brief Constructeur
/// \param pin Pin PWM lié au moteur
Vibrateur::Vibrateur(int pin) {
	_pin = pin;
}
/// \brief Cette methode initialise le pin pour le vibrateur
void Vibrateur::init(){
	pinMode(_pin, OUTPUT);
}
/// \brief Cette methode arrete la vibration du systeme
void Vibrateur::stop(){
	analogWrite(_pin, 0);
}
/// \brief Cette methode commence la vibration
/// \param power valeur entre 0 et 100
void Vibrateur::vibrate(int power){
	// power goes from 0 to 100%
	int p = map(power, 0, 100, 0, 255);
	analogWrite(_pin, p);
}
/// \brief Cette methode commence la vibration et l'arrete apres un delay
/// \param power Valeur entre 0 et 100
/// \param delay_ Valeur de temps en milliseconds
void Vibrateur::vibrate_time(int power, int delay_){
	// power goes from 0 to 100%
	int p = map(power, 0, 100, 0, 255);
	analogWrite(_pin, p);
	delay(delay_);
	analogWrite(_pin, 0);
}
