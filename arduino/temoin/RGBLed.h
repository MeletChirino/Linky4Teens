#ifndef RGBLed_h
#define RGBLed_h
#include "Buzzer.h"
/** 
 * \class RGBLed
 * \brief Classe qui gere la LED RGB
 * \details Cette classe gere la LED RG et fait plus 
 * facilement sa configuration et utilisation. Il faut que les
 * pins utilisés soient des pins PWM.
 */
class RGBLed{
	private:
		int _pinRed, _pinGreen, _pinBlue;
		int _red, _green, _blue;
    int _buzz;
    bool _buzz_state;
	public:
		RGBLed(int, int, int);
    RGBLed(int, int, int, int);
		void init();
		void set_color(int, int, int);
		void set_blue();
		void set_yellow();
		void set_green();
		void set_red();
		void lights_on();
		void blink_red(int);
		void test();
    void blink_green(int);
};

#endif
