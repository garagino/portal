const int presenceSensorPin = 2;

void setup() {
  Serial.begin(9600);
  pinMode(presenceSensorPin, INPUT);
}

void loop() {
  Serial.println(digitalRead(presenceSensorPin));
}
