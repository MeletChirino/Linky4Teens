//----------------- SD card config -----------------
#include <SD.h>
#include <SPI.h>

File myFile;
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

const byte interruptPin = 2;
volatile byte state = LOW;
int delta = 0;
double new_millis = 0, last_millis = 0;
int status_ = 0;

void setup() {
  Serial.begin(115200);

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
  save_info();
  switch (status_) {
    case 0:
      Serial.print("Waiting to start the race ");
      while (status_ == 0);
      break;

    // ******************* race mode *******************
    case 1:
      Serial.println(" in race ");
      led_rgb.set_yellow();

      while (status_ == 1) {
        vibz.stop();
        grip_sensor.read_grip();
        grip_sensor.print_status();
        if (grip_sensor.grip_ok())
          led_rgb.set_red();
        else
          led_rgb.set_blue();
      }
      break;

    // ******************* start transmission zone *******************
    case 2://transmission zone started
      Serial.print(" start zone ");

      while (status_ == 2 ) {

        float vib_pow;
        vib_pow = sin(0.1*millis());
        vib_pow = map(vib_pow, -1, 1, 0, 255);
        vibz.vibrate(vib_pow);
        
        if (grip_sensor.good_relay()) {
          led_rgb.set_green();
        } else {
          led_rgb.set_yellow();
        }
      }

      break;
    // ******************* end transmission zone *******************
    case 3:
      Serial.print(" end race ");
      vibz.stop();
      led_rgb.set_blue();
      break;

    // ******************* end race *******************
    case 4:
      Serial.print(" end zone ");
      vibz.stop();
      led_rgb.set_green();
      break;

  }
  Serial.print("delta = ");
  Serial.println(delta);
  delay(30);
}

//IR detecting protocol by interrupts
void detect_zone() {
  new_millis = millis();
  delta = new_millis - last_millis;
  last_millis = new_millis;


  if (delta == 47)
    status_ = 1;
  else if ( delta == 72)
    status_ = 2;
  else if ( delta == 118)
    status_ = 2;
  else if (delta == 32)
    status_ = 4;
}

// funciton to save information
void save_info() {
  myFile = SD.open("data.txt", FILE_WRITE);
  if (myFile) {
    myFile.print(status_);
    myFile.print(",");
    myFile.print(grip_sensor.low_grip());
    myFile.print(",");
    myFile.print(grip_sensor.high_grip());
    myFile.print(",");
    myFile.print(mma.x);
    myFile.print(",");
    myFile.print(mma.y);
    myFile.print(",");
    myFile.print(mma.z);
    myFile.print(",");
    myFile.println(millis());
    delay(1);
    myFile.close();
    Serial.println("written");
  } else {
    Serial.println("Error while saving info");
  }
}
