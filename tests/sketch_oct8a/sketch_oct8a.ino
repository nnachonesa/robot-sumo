bool detectado = true;
const int m1 = 4, m2 = 5,
/*
  m1=motor 1=motor izquierdo
  m2=motor 2=motor derecho
*/
sdl1=14, sdl2=13, sdl3=15, sdl4=16, trigp= 34;
void IRAM_ATTR onSensorDetectado() {
  while(digitalRead(sdl1) == LOW) {
      Serial.println("encendido");
      digitalWrite(m1, LOW);
        digitalWrite(m2, HIGH);
        detectado = true;
        }
}

void setup() {
  Serial.begin(115200);
  pinMode(14, INPUT);  
    pinMode(m1, OUTPUT);
    pinMode(m2, OUTPUT);
  attachInterrupt(digitalPinToInterrupt(14), onSensorDetectado, FALLING);
}

void loop() {
if(true) {
  
}
}

