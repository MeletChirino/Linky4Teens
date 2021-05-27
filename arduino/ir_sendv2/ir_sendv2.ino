/**
   @file ir_sendv2.ino

   @section description Description
	Cette appareil envoie des pulses vers le temoin dans la course pour savoir 
	la zone ou il se trouve. L'appareil a une conmutateur pour selectioner le
	comportement qu'il aura. Il a ete concu modulairement pour et donc nous pouvons 
	changer les LED juste en modifiant la saortie

   @section circuit Circuit
	- Il y a une circuit qui fait une onde carre de 38KHz et le microcontroleur le pilote
	avec un transisteur. L'onde carre fait rouler la LED IR pour envoye une signal reconnue
	par le recepteur IR dans le temoin.
	-Le module du led Peut etre change pour une autre LED qui soit plus ou moins puissante.
	-Le micrcontroleur marche avec un regulateur de 5V alors que le reste du circuit marche 
	avec la tension de la batterie.

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
