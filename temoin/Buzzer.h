#ifndef Buzzer_h
#define Buzzer_h

class Buzzer{
	public:
		int buzzer_pin;
		float melody_time;
		Buzzer(int);
		void init();
		void raw_tone(int, int);
		void test();
		void victory();
		void raw_tone_worst(int);
		/* we recommend you not to use 
		 * this one unless you know
		 * what you're doing 'cause
		 * it can be freaking annoying
		 */
		void stop();
		void transition_zone();
};
#endif
