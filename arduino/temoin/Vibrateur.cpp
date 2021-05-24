#include "Arduino.h"
#include "Vibrateur.h"

Vibrateur::Vibrateur(int pin) {
	_pin = pin;
}
void Vibrateur::init(){
	pinMode(_pin, OUTPUT);
}
void Vibrateur::stop(){
	analogWrite(_pin, 0);
}
void Vibrateur::vibrate(int power){
	// power goes from 0 to 100%
	int p = map(power, 0, 100, 0, 255);
	analogWrite(_pin, p);
}

void Vibrateur::vibrate_time(int power, int delay_){
	// power goes from 0 to 100%
	int p = map(power, 0, 100, 0, 255);
	analogWrite(_pin, p);
	delay(delay_);
	analogWrite(_pin, 0);
}
