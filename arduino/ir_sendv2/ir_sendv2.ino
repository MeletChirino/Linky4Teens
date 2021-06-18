/**
   @file ir_sendv2.ino

   @section description Description
   An example sketch demonstrating how to use Doxygen style comments for
   generating source code documentation with Doxygen.

   @section circuit Circuit
   - Red LED connected to pin D2.
   - Momentary push button connected to pin D3.

   @section libraries Libraries
   - Arduino_LSM6DS3 (https://github.com/arduino-libraries/Arduino_LSM6DS3)
     - Interacts with on-board IMU.

   @section notes Notes
   - Comments are Doxygen compatible.

   @section todo TODO
   - Don't use Doxygen style formatting inside the body of a function.

   @section author Author
   - Created by Melet Chirino on 12/05/2021.
*/
const int ledPin = 12;/*!< Pin attaché à la LED IR */

const int race = 10;/*!< Pin attaché à la signal de l'interrupteur pour savoir si la barriere IR agit sur le course ou la zone de transmission */
const int start = 11;/*!< Pin attaché à la signal de l'interrupteur pour savoir si la barriere IR sur le debut ou la fin d'une zone */

const int battery_pins[8] = {9, 8, 7, 6, 5, 4, 3, 2};
/// \brief Fonction par defaut d'arduino pour initialiser les pins
void setup() {

  Serial.begin(115200);

  pinMode(ledPin, OUTPUT);
  pinMode(race, INPUT);
  pinMode(start, INPUT);

  for (int i = 0; i < 8; i++) {
    pinMode(battery_pins[i], OUTPUT);
    digitalWrite(battery_pins[i], LOW);
  }
  delay(3000);

  for (int i = 0; i < 8; i++) {

    digitalWrite(battery_pins[i], HIGH);
    delay(100);
  }
  delay(1000);
  for (int i = 8; i >= 0; i -= 1) {

    digitalWrite(battery_pins[i], LOW);
    delay(100);
  }
  delay(1000);

  digitalWrite(9, LOW);

  digitalWrite(ledPin, 0);
}

/// \brief Fonction par defaut d'arduino qui s'execute en loop
void loop() {
  int battery_charge;
  battery_charge = analogRead(0);
  battery_charge = map(battery_charge, 0, 1023, 0, 100);
  battery_show(battery_charge);
  Serial.println(analogRead(0));

  if (digitalRead(race)) {
    if (digitalRead(start)) {
      blink_(47);
    } else {
      blink_(72);
    }
  } else {
    if (digitalRead(start)) {
      blink_(118);
    } else {
      blink_(32);
    }
  }
}

/// \brief Fonction qui clignote une LED
/// \param delay_ Le delay que la LED s'allume et s'eteint
void blink_(int delay_) {
  digitalWrite(ledPin, 1);
  delay(delay_);
  digitalWrite(ledPin, 0);
  delay(delay_);
}
