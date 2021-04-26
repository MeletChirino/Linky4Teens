//----------------- IR receiver config -----------------
#include "IrReceiver.h"
/* infrared receiver pin */
int ir_capture_pin = 14;
IrReceiver ir_receiver(ir_capture_pin);
//----------------- IR receiver config -----------------

//----------------- Grip sensor config -----------------
#include "GripSensor.h"
/* position pins */
GripSensor ldr_sensor(7, 8);
//----------------- Grip sensor config -----------------

//----------------- Buzzer config -----------------
#include "Buzzer.h"
/* buzzer config */
Buzzer buzz(6);//this is pin number of buzzer
//----------------- Buzzer config -----------------

//----------------- RGB Led config -----------------
#include "RGBLed.h"
/* pins for RGB Led */
RGBLed led_rgb(
  11,//red
  9,//green
  10,//blue
  buzz.buzzer_pin
);
//----------------- RGB Led config -----------------

//----------------- Race stats -----------------
//this variable knows how many relays have been done while the race
int lap = 0;
int max_laps = 5;

/*
  A simple matrix that holds some stats of the race:
  t1 -> time to arrive to transition zone
  t2 -> time inside transmission zone before the relay
  t3 -> time inside transmission zone after relay
  relay_state -> it says whether this relay was good or not
  [t1, t2, t3, relay_state]
  it can hold up to 6 relays in this version, can be upgraded if needed
*/
float race_stats[4][5];

//this variable saves if any grip errorr is made
float grip_errors[40];
/* transition zone flag */
bool transition_zone = false;
//----------------- Race stats -----------------

//----------------- Serial parameters -----------------
String inputString = "";         // a String to hold incoming data
bool stringComplete = false, send_data = false;  // whether the string is complete
//----------------- Serial parameters -----------------



void setup() {
  // initialize serial:
  Serial.begin(115200);

  // reserve 200 bytes for the inputString:
  inputString.reserve(200);

  //initialize buzzer
  buzz.init();

  // Initialize interrupt service routine
  ir_receiver.init();

  // initialize position detector pins
  ldr_sensor.init();

  // initialize RGBPins
  led_rgb.init();
  led_rgb.set_color(100, 0, 100);

  //delay(10000);
  led_rgb.test();
}

void loop() {

  if (lap >= max_laps) {
    buzz.stop();
    led_rgb.set_color(100, 0, 100);
    /*
      for (int i = 0; i < max_laps; i++) {
      Serial.print("\n\nLap "); Serial.println(i);
      Serial.print("t1 "); Serial.print(race_stats[0][i]);
      Serial.print("t2 "); Serial.print(race_stats[1][i]);
      Serial.print("t3 "); Serial.print(race_stats[2][i]);
      Serial.print("ok = "); Serial.println(race_stats[3][i]);
      }
      delay(10000);
    */
    if (send_data) {
      for (int i = 0; i < max_laps; i++) {
        Serial.print("s");
        Serial.print("/");
        Serial.print(race_stats[0][i]);
        Serial.print("/");
        Serial.print(race_stats[1][i]);
        Serial.print("/");
        Serial.print(race_stats[2][i]);
        Serial.print("/");
        Serial.print (race_stats[3][i]);
        Serial.print("/");
        Serial.println("s");
      }
      send_data = 0;
    }
    if (inputString == "ok?\n") Serial.println("ok");
    if (inputString == "start\n") {
      //Serial.println("toogle transmisison");
      if (send_data) send_data = 0;
      else send_data = 1;
      //Serial.println("send_data != send_data");
    }
    // print the string when a newline arrives:
    if (stringComplete) {
      //Serial.println(inputString);
      // clear the string:
      inputString = "";
      stringComplete = false;
    }
    return;
  }

  while (!transition_zone) {
    buzz.stop();
    //while not in transition zone
    ldr_sensor.read_grip();
    ldr_sensor.print_status(transition_zone);

    if (ldr_sensor.grip_ok())
      led_rgb.set_red();
    else
      led_rgb.set_blue();

    if (ir_receiver.get_status()) {
      transition_zone = true;
      buzz.melody_time = millis();
      race_stats[0][lap] = millis();
      led_rgb.blink_green(2);
      //delay(500);
    }
  }

  while (transition_zone) {
    //while in transition zone
    ldr_sensor.print_status(transition_zone);

    if (ldr_sensor.good_relay()) {
      buzz.victory();
      led_rgb.set_green();
      if (race_stats[1][lap] == 0) {
        race_stats[1][lap] = millis();
        race_stats[3][lap] = 1;
      }
    } else {
      buzz.transition_zone();
      led_rgb.set_yellow();
      race_stats[3][lap] = 0;
    }
    if (ir_receiver.get_status()) {
      ldr_sensor.reset_relay(led_rgb);
      transition_zone = false;
      race_stats[2][lap] = millis();
      lap++;
      led_rgb.blink_green(2);
      //delay(500);
    }

  }
  /*
    Serial.print("Lap "); Serial.println(lap);
    Serial.print("t1 "); Serial.print(race_stats[0][lap-1]);
    Serial.print("| t2 "); Serial.print(race_stats[1][lap-1]);
    Serial.print("| t3 "); Serial.print(race_stats[2][lap-1]);
    Serial.print("| ok = "); Serial.println(race_stats[3][lap-1]);
  */


  //Serial.println(millis());
}

void change_zone() {
  transition_zone = !transition_zone;
  Serial.println("interrupt");
  delay(300);
}

/*
  SerialEvent occurs whenever a new data comes in the hardware serial RX. This
  routine is run between each time loop() runs, so using delay inside loop can
  delay response. Multiple bytes of data may be available.
*/
void serialEvent() {
  //Serial.println("Serial Event!!\n");
  //serial_blink(4, 300);
  while (Serial.available()) {
    // get the new byte:
    char inChar = (char)Serial.read();
    // add it to the inputString:
    inputString += inChar;
    // if the incoming character is a newline, set a flag so the main loop can
    // do something about it:
    if (inChar == '\n') {
      stringComplete = true;
    }
  }
}
