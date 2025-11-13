#include <Arduino.h>

// Pines
const int m1 = 4, m2 = 5, m3 = 22, m4 = 23; // L298N señales
const int sdl13 = 26, sdl14 = 27;  // Sensores de línea restantes
const int trigp = 17, echop = 16;
const int led1 = 18, led2 = 19, led3 = 21;
const int botoninit = 33;

float dis = 1000;
unsigned long startFightTime = 0; // Variables descriptivas para controlar el tiempo de pelea
bool fightEnabled = false;

enum State {
  WAITING,
  COUNTDOWN,
  EXPLOSIVE,
  SAFE_FIGHT
}; 
State robotState = WAITING;

// Funciones motores
void stopMotors(){
  digitalWrite(m1, LOW); digitalWrite(m2, LOW);
  digitalWrite(m3, LOW); digitalWrite(m4, LOW);
}

void forward(){
  digitalWrite(m1, LOW); digitalWrite(m2, LOW);
  digitalWrite(m3, HIGH); digitalWrite(m4, HIGH);
}

void backward(){
  digitalWrite(m1, HIGH); digitalWrite(m2, HIGH);
  digitalWrite(m3, LOW); digitalWrite(m4, LOW);
}

void turnRight(){
  digitalWrite(m1, HIGH); digitalWrite(m2, LOW);
  digitalWrite(m3, LOW);  digitalWrite(m4, HIGH);
}

void turnLeft(){
  digitalWrite(m1, LOW);  digitalWrite(m2, HIGH);
  digitalWrite(m3, HIGH); digitalWrite(m4, LOW);
}

// Función para leer los sensores de línea
bool checkLineSensor() {
  return (digitalRead(sdl13) == LOW || digitalRead(sdl14) == LOW);
}

// Función para leer el ultrasonido
float readUltrasonic() {
  digitalWrite(trigp, LOW);
  delayMicroseconds(2);
  digitalWrite(trigp, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigp, LOW);

  unsigned long duration = pulseIn(echop, HIGH, 20000);
  if(duration > 0) return duration * 0.034 / 2;
  else return 200; // Si no se detecta nada
}

// Setup
void setup() {
  pinMode(m1, OUTPUT); pinMode(m2, OUTPUT);
  pinMode(m3, OUTPUT); pinMode(m4, OUTPUT);

  pinMode(sdl13, INPUT); pinMode(sdl14, INPUT);  // Sensores de línea
  pinMode(trigp, OUTPUT); pinMode(echop, INPUT);
  pinMode(led1, OUTPUT); pinMode(led2, OUTPUT); pinMode(led3, OUTPUT);
  pinMode(botoninit, INPUT);  // Botón de inicio

  Serial.begin(115200);  // Inicializa el puerto serial para depuración

  stopMotors();  // Detén los motores inicialmente
}

// Función para controlar el botón de inicio y las luces
void handleButtonAndLEDs() {
  if(digitalRead(botoninit) == HIGH && robotState == WAITING) {
    robotState = COUNTDOWN;
    digitalWrite(led1, HIGH); digitalWrite(led2, LOW); digitalWrite(led3, LOW);
    delay(50); 
    digitalWrite(led1, LOW); digitalWrite(led2, HIGH);
    delay(50);

    fightEnabled = true;
    robotState = EXPLOSIVE;
    startFightTime = millis();
  }

  if(robotState == WAITING) {
    digitalWrite(led1, HIGH); digitalWrite(led2, LOW); digitalWrite(led3, LOW); // Rojo
  } else if(robotState == COUNTDOWN){
    digitalWrite(led1, LOW); digitalWrite(led2, HIGH); digitalWrite(led3, LOW); // Verde
  } else if(robotState == EXPLOSIVE || robotState == SAFE_FIGHT){
    digitalWrite(led1, LOW); digitalWrite(led2, LOW); digitalWrite(led3, HIGH); // Azul
  }
}

// Función para controlar el movimiento de acuerdo al estado de la pelea
void handleCombat() {
  if(!fightEnabled) {
    stopMotors();
    return;
  }

  if(robotState == EXPLOSIVE) {
    forward();
    if(millis() - startFightTime >= 2500) {
      robotState = SAFE_FIGHT;
    }
  } else if(robotState == SAFE_FIGHT) {
    dis = readUltrasonic();
    if(dis <= 30) {
      forward(); // Atacar
    } else {
      turnRight(); // Buscar al oponente
    }
  }
}

// Función para manejar los sensores de línea
void handleLineSensors() {
  if(checkLineSensor() && fightEnabled) {
    backward();
    delay(300);  // Retrocede 300ms
    turnRight();
    delay(300);  // Gira 300ms
  }
}

void loop() {
  handleButtonAndLEDs();  // Control del botón y LEDs
  handleCombat();         // Control del combate
  handleLineSensors();    // Control de los sensores de línea
}
