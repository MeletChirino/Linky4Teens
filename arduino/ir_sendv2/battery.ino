void battery_show(int charge) {
  int charge_;
  charge_ = map(charge, 0, 100, 0, 8);
    Serial.print("charge = ");
  Serial.print(charge);
  Serial.print("% pins = ");
  Serial.println(charge_);
  for (int i = charge_; i >= 0; i -= 1) {
    digitalWrite(battery_pins[i], LOW);
  }
    for (int i = charge_; i < 8; i ++) {
    digitalWrite(battery_pins[i], HIGH);
  }
}
