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
const int ledPin = 7;/*!< Pin attaché à la LED IR */

const int race = 18;/*!< Pin attaché à la signal de l'interrupteur pour savoir si la barriere IR agit sur le course ou la zone de transmission */
const int start = 13;/*!< Pin attaché à la signal de l'interrupteur pour savoir si la barriere IR sur le debut ou la fin d'une zone */

/// \brief Fonction par defaut d'arduino pour initialiser les pins
void setup() {

  pinMode(ledPin, OUTPUT);
  pinMode(race, INPUT);
  pinMode(start, INPUT);

  digitalWrite(ledPin, 0);
}

/// \brief Fonction par defaut d'arduino qui s'execute en loop
void loop() {

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
