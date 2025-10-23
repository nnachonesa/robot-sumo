const int ledPin = 2; // The built-in LED on some ESP32 boards is on GPIO2

void setup() {
  // initialize digital pin ledPin as an output.
  pinMode(ledPin, OUTPUT);
}

void loop() {
  digitalWrite(ledPin, HIGH); // Turn the LED on (HIGH is the voltage level)
  delay(1000);                // Wait for a second (1000 milliseconds)
  digitalWrite(ledPin, LOW);  // Turn the LED off by making the voltage LOW
  delay(1000);                // Wait for a second
}