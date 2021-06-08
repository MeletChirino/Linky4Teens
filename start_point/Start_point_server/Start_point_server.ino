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
        Serial.println(millis() - start_millis);
        client.print("l_force_1");//left force 1
        client.print(",");
        client.print("l_force_2");// left force 2
        client.print(",");
        client.print("l_force_3");// left force 3
        client.print(",");
        client.print("l_force_4");// left force 4
        client.print(",");
        //Right Force
        client.print("r_force_1");//right force 1
        client.print(",");
        client.print("r_force_2");//right force 2
        client.print(",");
        client.print("r_force_3");
        client.print(",");
        client.print("r_force_4");
        client.print(",");
        client.println("time");
        while (millis() - start_millis < 5000) {
          //Left Force
          Serial.println(millis() - start_millis);
          client.print(millis() - start_millis + 20);//left force 1
          client.print(",");
          client.print(millis());// left force 2
          client.print(",");
          client.print(sin(millis()));// left force 3
          client.print(",");
          client.print(millis() - start_millis + 80);// left force 4
          client.print(",");
          //Right Force
          client.print(millis());//right force 1
          client.print(",");
          client.print(sin(millis()));//right force 2
          client.print(",");
          client.print(millis());
          client.print(",");
          client.print(sin(millis()));
          client.print(millis() - start_millis + 50);
          client.print(",");
          client.println(millis() - start_millis);

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
