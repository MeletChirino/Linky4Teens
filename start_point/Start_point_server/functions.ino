void send_data(WiFiClient client, float val_1, float val_2, float time_) {
  //Left Force
  client.print(val_1);//left force 1
  //Right Force
  client.print(",");
  client.print(val_2);//right force 1
  //time
  client.print(",");
  client.println(millis() - time_);
  //Left Force
  Serial.print(val_1);//left force 1
  //Right Force
  Serial.print(",");
  Serial.print(val_2);//right force 1
  //time
  Serial.print(",");
  Serial.println(millis() - time_);
}
void send_header(WiFiClient client) {
  //Left Force
  client.print("l_force");//left force 1
  //Right Force
  client.print(",");
  client.print("r_force");//right force 1
  //time
  client.print(",");
  client.println("time");
}
void send_battery_charge(WiFiClient client) {
  //this function reads battery charge in pin 35 and maps it
  //to a percentage value
  int volts;
  //volts = analogRead(battery_pin);
  //volts = map(volts, 3598, 4095, 0, 100);
  volts = 62;
  Serial.println(volts);
  client.println(volts);
}
