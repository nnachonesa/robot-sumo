const int m1=5, m2=4, seg=14;
bool pm1=false, pm2=false;
// esta funcion va a detectar si sensor de linea
void taskLineDetect(void *pvParameters) {
  while(true) {
if(digitalRead(seg) == 0) {
    pm1 = false;
    pm2 = true;
  } else {
    pm1 = false;
    pm2 = false;
  }
  }
}
// otra funcion para que avance
void taskGo(void *pvParameters) {
  digitalWrite(m1, pm1);
  digitalWrite(m2, pm2);
} 

void setup() {
  // put your setup code here, to run once:
  pinMode(m1, OUTPUT);
  pinMode(m2, OUTPUT);
  pinMode(seg, INPUT);

  // constructor de las tareas
  xTaskCreatePinnedToCore(taskLineDetect, "Deteccion de linea", 1000, NULL, 1, NULL, 1);
  xTaskCreatePinnedToCore(taskGo, "Avance", 1000, NULL, 1, NULL, 1);
}

void loop() {
  // put your main code here, to run repeatedly:

}
