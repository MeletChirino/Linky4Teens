#include "Arduino.h"
#include "Buzzer.h"

Buzzer::Buzzer(int buzzer_pin_){
	//constructor
	buzzer_pin = buzzer_pin_;
}
void Buzzer::init(){
	pinMode(buzzer_pin, OUTPUT);
	digitalWrite(buzzer_pin, 0);// turn off buzzer(its anoying)	
}
void Buzzer::raw_tone(int delay_, int tone_) {
  analogWrite(buzzer_pin, tone_);
  delay(delay_);
  digitalWrite(buzzer_pin, 0);
}
void Buzzer::test(){
	raw_tone(400, 88);
	raw_tone(400,115);
	raw_tone(200,200);
}
void Buzzer::transition_zone(){
	float time_ = millis() - melody_time;
	if(time_ <= 400)
		analogWrite(buzzer_pin, 88);
	else if( time_ > 400 && time_ < 600)
		digitalWrite(buzzer_pin, 0);//analogWrite(buzzer_pin, 160);
		//analogWrite(buzzer_pin, 160);
	else if(time_ > 600)
		melody_time = millis();
}
void Buzzer::victory(){
	float time_ = millis() - melody_time;
	if(time_ <= 600)
		analogWrite(buzzer_pin, 40);
	else if( time_ > 600 && time_ < 1000)
		digitalWrite(buzzer_pin, 0);//analogWrite(buzzer_pin, 160);
	else if(time_ > 1000)
		melody_time = millis();
}
void Buzzer::raw_tone_worst(int tone_){
	analogWrite(buzzer_pin, tone_);
}
void Buzzer::stop(){
	digitalWrite(6, 0);
}
