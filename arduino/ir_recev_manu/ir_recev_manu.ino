#include <IRremote.h>
const int PIN = 4;

IRrecv receiver(PIN);
decode_results results;

const int ledPin = 8;

void setup() {
  Serial.begin(9600);
  Serial.println("Start!\n");

  receiver.enableIRIn();

  pinMode(ledPin, OUTPUT);
  digitalWrite(ledPin, 0);
  blink_(5);
}

void loop() {
  if (receiver.decode(&results)) {
    Serial.println(results.value, BIN);
    Serial.println(results.value, HEX);
    switch (results.decode_type) {
      case NEC:
        Serial.println("NEC");
        blink_(2);
        break;
      case SONY:
        Serial.println("SONY");
        break;
      case RC5:
        Serial.println("RC5");
        break;
      case RC6:
        Serial.println("RC6");
        blink_(4);
        break;
      case DISH:
        Serial.println("DISH");
        break;
      case SHARP:
        Serial.println("SHARP");
        break;
      case JVC:
        Serial.println("JVC");
        break;
      //case SANYO:
      //  Serial.println("SANYO");
      //  break;
      //case MITSUBISHI:
      //  Serial.println("MITSUBISHI");
      //  break;
      case SAMSUNG:
        Serial.println("SAMSUNG");
        break;
      case LG:
        Serial.println("LG");
        break;
      case WHYNTER:
        Serial.println("WHYNTER");
        break;
      //case AIWA_RC_T501:
      //  Serial.println("AIWA_RC_T501");
      //  break;
      case PANASONIC:
        Serial.println("PANASONIC");
        break;
      case DENON:
        Serial.println("DENON");
        break;
      default:
      case UNKNOWN:
        Serial.println("UNKNOWN");
        break;
    }
    receiver.resume();
  }
}

void blink_(int times) {
  int delay_ = 100;
  for (int i = 0; i < times; i++) {
    digitalWrite(ledPin, 1);
    delay(delay_);
    digitalWrite(ledPin, 0);
    delay(delay_);
  }
}
