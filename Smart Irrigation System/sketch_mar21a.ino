void setup() {
  pinMode(soilSensor, INPUT);
  pinMode(relayPin, OUTPUT);
  Serial.begin(9600);
}

void loop() {
  int moisture = analogRead(soilSensor);  
  Serial.print("Soil Moisture: ");
  Serial.println(moisture);

  if (moisture < 400) {  // Adjust threshold based on testing
    digitalWrite(relayPin, HIGH);  // Turn ON pump
    Serial.println("Water Pump ON");
  } else {
    digitalWrite(relayPin, LOW);  // Turn OFF pump
    Serial.println("Water Pump OFF");
  }
  delay(2000);
}