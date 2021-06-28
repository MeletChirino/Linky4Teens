void send_data(WiFiClient client, float val_1, float val_2) {
  //Left Force
  client.print(val_1);//left force 1
  client.print(",");
  client.print(millis()  * 12);// left force 2
  client.print(",");
  client.print(sin(millis()));// left force 3
  client.print(",");
  client.print(millis() - 50);// left force 4
  client.print(",");
  //Right Force
  client.print(val_2);//right force 1
  client.print(",");
  client.print(sin(millis()) * 20000); //right force 2
  client.print(",");
  client.print(millis() / 1000);
  client.print(",");
  client.print(sin(millis()) * 3000);
  //time
  client.print(",");
  client.println(millis());
}
