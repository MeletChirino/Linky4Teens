//----------------- SD card config -----------------
#include <SD.h>
#include <SPI.h>

File data_file;
int pinCs = 8;
//----------------- SD card config -----------------

//----------------- Grip sensor config -----------------
#include "GripSensor.h"
/* position pins */
GripSensor grip_sensor(10, 7);
//----------------- Grip sensor config -----------------

//----------------- Accelerometer config -----------------
#include <Wire.h>
#include <Adafruit_MMA8451.h>
#include <Adafruit_Sensor.h>

Adafruit_MMA8451 mma = Adafruit_MMA8451();

//----------------- Accelerometer config -----------------


//----------------- RGB Led config -----------------
#include "RGBLed.h"
/* pins for RGB Led */
RGBLed led_rgb(
  6,//red
  3,//green
  9//blue
);
//----------------- RGB Led config -----------------

//----------------- Vibrateur config -----------------
#include "Vibrateur.h"
/* pins for Vibration motor*/
Vibrateur vibz(5);
//----------------- Vibrateur config -----------------

//----------------- Serial parameters -----------------
String inputString = "";         // a String to hold incoming data
bool stringComplete = false, send_data = false;  // whether the string is complete
//----------------- Serial parameters -----------------


const byte interruptPin = 2;
volatile byte state = LOW;
int delta = 0;
double new_millis = 0, last_millis = 0;
int status_ = 0;

void setup() {
  Serial.begin(115200);

  // reserve 200 bytes for the inputString:
  inputString.reserve(200);


  //grip sensore init
  grip_sensor.init();

  //RGB init
  led_rgb.init();
  led_rgb.test();
  led_rgb.set_color(100, 100, 100);

  //sd card init
  pinMode(pinCs, OUTPUT);

  //sd card init
  if (SD.begin()) {
    Serial.println("SD card Ready!!");
    SD.remove("data.txt");

  } else {
    Serial.println("Error with SD card");
    while (1);
  }

  //vibrateur init
  vibz.init();
  vibz.vibrate_time(50, 50);

  //interruption init
  pinMode(interruptPin, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(interruptPin), detect_zone, CHANGE);

  //Accelerometer init
  Serial.println("Adafruit MMA8451 test!");
  if (! mma.begin()) {
    Serial.println("Couldnt start");
    delay(200);
    while (1);
  }
  Serial.println("MMA8451 found!");

  mma.setRange(MMA8451_RANGE_2_G);

  Serial.print("Range = "); Serial.print(2 << mma.getRange());
  Serial.println("G");

  status_ = 1;
}



void loop() {
  int i;
  vibz.stop();

  if (status_ == 4) {
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
      data_file = SD.open("DATA.TXT");
      if (data_file) {
        while (data_file.available()) {
          Serial.write(data_file.read());
        }
        data_file.close();
      } else {
        Serial.println("E: Problem with SD card");
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

  switch (status_) {
    case 0:
      Serial.print("Waiting to start the race ");
      while (status_ == 0);
      break;

    // ******************* race mode *******************
    case 1:
      Serial.println(" status 1 ");
      led_rgb.set_yellow();

      while (status_ == 1) {
        save_info();
        vibz.stop();
        grip_sensor.read_grip();
        //rip_sensor.print_status();
        if (grip_sensor.grip_ok())
          led_rgb.set_red();
        else
          led_rgb.set_blue();
      }
      break;

    // ******************* start transmission zone *******************
    case 2://transmission zone started
      Serial.print(" start zone status 2 ");
      i = 0;
      while (status_ == 2 ) {
        save_info();
        float vib_pow;
        vib_pow = (1.5 + sin(i)) * 120;
        //Serial.print("Pow = "); Serial.println(vib_pow);
        vibz.vibrate(vib_pow);

        if (grip_sensor.good_relay()) {
          led_rgb.set_green();
        } else {
          led_rgb.set_yellow();
        }

        delay(100);
        i++;
      }

      break;
    // ******************* end transmission zone *******************
    case 3:
      save_info();
      Serial.print(" end transmission zone  status 3 ");
      vibz.stop();
      led_rgb.set_blue();
      for (i = 0; i < 2; i++) {
        vibz.vibrate_time(50, 50);
        delay(100);
      }
      status_ = 1;
      //If good relay you do this

      //else you do that

      //finally you go back to state = 2
      break;

    // ******************* end race *******************
    case 4:
      save_info();
      Serial.print(" status = 4 ");
      vibz.stop();
      led_rgb.set_color(100, 100, 100);
      //you set the program ready for sending the data to serial
      //while (1);
      break;

  }

  delay(30);
}

//IR detecting protocol by interrupts
void detect_zone() {
  new_millis = millis();
  delta = new_millis - last_millis;
  last_millis = new_millis;


  if (delta == 47)
    status_ = 1;//race start
  else if ( delta == 72)
    status_ = 2;//start
  else if ( delta == 118)
    status_ = 4;
  else if (delta == 32)
    status_ = 3;

  Serial.print("status = ");
  Serial.println(status_);
}

// funciton to save information
void save_info() {
  mma.read();
  data_file = SD.open("data.txt", FILE_WRITE);
  if (data_file) {
    data_file.print(status_);
    data_file.print(",");
    data_file.print(grip_sensor.low_grip());
    data_file.print(",");
    data_file.print(grip_sensor.high_grip());
    data_file.print(",");
    data_file.print(mma.x);
    data_file.print(",");
    data_file.print(mma.y);
    data_file.print(",");
    data_file.print(mma.z);
    data_file.print(",");
    data_file.println(millis());
    delay(1);
    data_file.close();
    //Serial.println("written");
  } else {
    Serial.println("Error while saving info");
  }
}
