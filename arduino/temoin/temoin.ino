/**
   @file temoin.ino

   @section description Description
   Ce sketch manage les etats du temoin pendant qu'il est en course 
   en plus il envoie les donnes enregistres dans la carte SD qui est
   dedans.

   @section circuit Circuit
   - Led RGB connecte aux pins 3, 6 et 9
	- Accelerometre connecte au bus I2C
	- Carte micro SD connecte au bus SPI
	- Capteurs touch connectes aux pins 10 et 7
	- Batterie rechargeable a 3.7V
	- Regulateur de tension qui augmente la tension jusqu'a 5V pour le microcontroleur
	- Moteur vibrateur connecte a travers d'un transisteur au pin 5
	- Connection vers le port serial d'une raspberry avec les pins RX et TX
	
   @section libraries Libraries
	- Librarie du Carte SD
	- Librarie de l'accelerometre

   @section notes Notes
	- Les circuits on ete developpes avec Proteus 8 mais il y a une version en Eagle 
	- Cette programme Envoie des donnes a une raspberry vers le port serial

   @section todo TODO
	- Ameliorer le protocol de communication IR ou utiliser une technologie plus robuste en conditions sportives

   @section author Author
   - Created by Melet Chirino on 30/04/2021.
*/

//----------------- SD card config -----------------
#include <SD.h>
#include <SPI.h>

File data_file; /*!< Objet pour les fichier dedans la carte SD */
int pinCs = 8; /*!< Pin pour la selection du carte SD avec le bus SPI */
//----------------- SD card config -----------------

//----------------- Grip sensor config -----------------
#include "GripSensor.h"
GripSensor grip_sensor(10, 7); /*!< Objet des capteur de grip */
//----------------- Grip sensor config -----------------

//----------------- Accelerometer config -----------------
#include <Wire.h>
#include <Adafruit_MMA8451.h>
#include <Adafruit_Sensor.h>

Adafruit_MMA8451 mma = Adafruit_MMA8451(); /*!< Acceleromettre connecté par le bus I2C */

//----------------- Accelerometer config -----------------


//----------------- RGB Led config -----------------
#include "RGBLed.h"

RGBLed led_rgb(
  6,//red
  3,//green
  9//blue
); /*!< Objet de la LED RGB */
//----------------- RGB Led config -----------------

//----------------- Vibrateur config -----------------
#include "Vibrateur.h"
Vibrateur vibz(5);/*!< Objet pour le moteur vibrateur */
//----------------- Vibrateur config -----------------

//----------------- Serial parameters -----------------
String inputString = ""; /*!< a String to hold incoming data */
bool stringComplete = false, send_data = false;/*!< whether the string is complete*/
//----------------- Serial parameters -----------------


const byte interruptPin = 2; /*!< Pin d'interruption utilisé pour le recepteur InfraRouge */
int delta = 0; /*!< Variable utilisé pour savoir le type de zone capté avec la barrierre InfraRouge */
double new_millis = 0, last_millis = 0;
int status_ = 0; /*!< Il y a un status pour savoir la partie de la course où se trouve le temoin */

/// \brief Fonction par defaut d'arduino pour initialiser toutes les variables.
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


/// \brief Fonction par defaut d'arduino qui s'execute en loop de mode indefini
void loop() {
  int i;
  vibz.stop();

  if (status_ == 4) {
    led_rgb.set_color(100, 0, 100);
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

/// \brief Protocol de detection des barrieres IR
/// \param delta=47 Debut de la course
/// \param delta=72 Debut de la zone de transmission
/// \param delta=118 Fin de la zone de trasnmission
/// \param delta=32 Fin de la Course
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

/// \brief Fonction pour enregistrer les donnés dans la carte SD
/* \detail Le microcontroleur enregistre les donnés en format CSV avec le format suivant :
 *  status, low_grip, high_grip, accel_x, accel_y, accel_z, milliseconds.
 */
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
