#include <WiFi.h>

const char* ssid     = "Linky4Teens Dock!";
const char* password = "linkypass!";

WiFiServer wifiServer(50);

void setup() {

  Serial.begin(115200);

  delay(1000);

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

  WiFiClient client = wifiServer.available();

  if (client) {

    while (client.connected()) {

      while (client.available() > 0) {
        c = client.read();
        Serial.write(c);
      }
      if (c == '1') {
        for (int i = 0; i < 2000; i++) {
          //Serial.println(i);
          client.print(millis());
          client.print(",");
          client.println(sin(millis()));
        }

      } else if (c == '2') {
        for (int i = 0; i < 2000; i++) {
          //Serial.println(i);
          client.print(sin(millis()));
          client.print(",");
          client.println("23");
        }
      }
      client.print("!");

      delay(10);
    }

    client.stop();
    Serial.println("\nClient disconnected");

  }
}
