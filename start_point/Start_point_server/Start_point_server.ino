#include <WiFi.h>
#include <EEPROM.h>
#include "HX711_ADC.h"

//HX711 pins
const int HX711_dout_1 = 21; //mcu > HX711 no 1 dout pin
const int HX711_sck_1 = 22; //mcu > HX711 no 1 sck pin
const int HX711_dout_2 = 15; //mcu > HX711 no 2 dout pin
const int HX711_sck_2 = 2;//mcu > HX711 no 2 sck pin

//Creating load cell objects
HX711_ADC LoadCell_1(HX711_dout_1, HX711_sck_1);
HX711_ADC LoadCell_2(HX711_dout_2, HX711_sck_2);

//eeprom address for calibration of each cell
const int calVal_eepromAdress_1 = 0; //  1 (4 bytes)
const int calVal_eepromAdress_2 = 4; // 2 (4 bytes)
unsigned long t = 0;

//Server config
const char* ssid     = "Linky4Teens Dock!";
const char* password = "linkypass!";

WiFiServer wifiServer(50);

void setup() {

  Serial.begin(115200);

  delay(1000);

  float calibrationValue_1; // calibration value load cell 1
  float calibrationValue_2; // calibration value load cell 2

  calibrationValue_1 = 10.7; // uncomment this if you want to set this value in the sketch
  calibrationValue_2 = 4.5; // uncomment this if you want to set this value in the sketch

  LoadCell_1.begin();
  LoadCell_2.begin();

  unsigned long stabilizingtime = 2000; // tare preciscion can be improved by adding a few seconds of stabilizing time
  boolean _tare = true; //set this to false if you don't want tare to be performed in the next step

  byte loadcell_1_rdy = 0;
  byte loadcell_2_rdy = 0;

  while ((loadcell_1_rdy + loadcell_2_rdy) < 2) { //run startup, stabilization and tare, both modules simultaniously
    if (!loadcell_1_rdy) loadcell_1_rdy = LoadCell_1.startMultiple(stabilizingtime, _tare);
    if (!loadcell_2_rdy) loadcell_2_rdy = LoadCell_2.startMultiple(stabilizingtime, _tare);
  }
  if (LoadCell_1.getTareTimeoutFlag()) {
    Serial.println("Timeout, check MCU>HX711 no.1 wiring and pin designations");
  }
  if (LoadCell_2.getTareTimeoutFlag()) {
    Serial.println("Timeout, check MCU>HX711 no.2 wiring and pin designations");
  }
  LoadCell_1.setCalFactor(calibrationValue_1); // user set calibration value (float)
  LoadCell_2.setCalFactor(calibrationValue_2); // user set calibration value (float)

  Serial.println("Startup is complete");


  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi..");
  }

  Serial.println("Connected to the WiFi network");
  Serial.println(WiFi.localIP());

  wifiServer.begin();
}
char c;
void loop() {

  long start_millis;
  WiFiClient client = wifiServer.available();

  if (client) {

    while (client.connected()) {

      while (client.available() > 0) {
        c = client.read();
        Serial.write(c);
      }
      if (c == '1') {
        start_millis = millis();
        send_header(client);
        while (millis() - start_millis < 5000) {
          static boolean newDataReady = 0;
          const int serialPrintInterval = 0; //increase value to slow down serial print activity

          // check for new data/start next conversion:
          if (LoadCell_1.update()) newDataReady = true;
          LoadCell_2.update();

          //get smoothed value from data set
          if ((newDataReady)) {
            if (millis() > t + serialPrintInterval) {
              float a = LoadCell_1.getData();
              float b = LoadCell_2.getData();
              send_data(client, a, b, start_millis); 
              //Serial.print("Load_cell 1 output val: ");
              //Serial.print(a);
              //Serial.print("    Load_cell 2 output val: ");
              //Serial.println(b);
              newDataReady = 0;
            }
          }
          // receive command from serial terminal, send 't' to initiate tare operation:
          if (Serial.available() > 0) {
            char inByte = Serial.read();
            if (inByte == 't') {
              LoadCell_1.tareNoDelay();
              LoadCell_2.tareNoDelay();
            }
          }

          //check if last tare operation is complete
          if (LoadCell_1.getTareStatus() == true) {
            Serial.println("Tare load cell 1 complete");
          }
          if (LoadCell_2.getTareStatus() == true) {
            Serial.println("Tare load cell 2 complete");
          }

        }

      } else if (c == '2') {
        client.println("ok");
      }
      client.print("!");

      delay(10);
    }

    client.stop();
    Serial.println("\nClient disconnected");

  }
}
