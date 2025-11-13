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
  digitalWrite(m1, HIGH); digitalWrite(m2, LOW);
  digitalWrite(m3, HIGH); digitalWrite(m4, LOW);
}

void backward(){
  digitalWrite(m1, LOW); digitalWrite(m2, HIGH);
  digitalWrite(m3, LOW); digitalWrite(m4, HIGH);
}

void turnRight(){
  digitalWrite(m1, HIGH); digitalWrite(m2, LOW);
  digitalWrite(m3, LOW);  digitalWrite(m4, HIGH);
}

void turnLeft(){
  digitalWrite(m1, LOW);  digitalWrite(m2, HIGH);
  digitalWrite(m3, HIGH); digitalWrite(m4, LOW);
}

// Tarea sensores de línea (ahora solo usa sdl13 y sdl14)
void TaskLineSensor(void *pv){
  for(;;){
    bool linea = (digitalRead(sdl13) == LOW || digitalRead(sdl14) == LOW);

    if(linea && fightEnabled){
      backward();
      vTaskDelay(300 / portTICK_PERIOD_MS);
      turnRight();
      vTaskDelay(300 / portTICK_PERIOD_MS);
    }

    vTaskDelay(1);
  }
}

// Tarea ultrasonido
void TaskUltrasonic(void *pv){
  for(;;){
    digitalWrite(trigp, LOW);
    delayMicroseconds(2);
    digitalWrite(trigp, HIGH);
    delayMicroseconds(10);
    digitalWrite(trigp, LOW);

    unsigned long duration = pulseIn(echop, HIGH, 20000);
    if(duration > 0) dis = duration * 0.034 / 2;
    else dis = 200; // Nada detectado

    vTaskDelay(30 / portTICK_PERIOD_MS);
  }
}

// Tarea de lógica de combate
void TaskCombat(void *pv){
  for(;;){
    if(!fightEnabled){
      stopMotors();
      vTaskDelay(10);
      continue;
    }

    // 2.5s inicial
    if(robotState == EXPLOSIVE){
      forward();
      if(millis() - startFightTime >= 2500){
        robotState = SAFE_FIGHT;
      }
    }
    else if(robotState == SAFE_FIGHT){
      if(dis <= 30){
        forward(); // Atacar
      } else {
        turnRight(); // Buscar
      }
    }

    vTaskDelay(10);
  }
}

// Tarea para el botón de inicio
void TaskStart(void *pv){
  for(;;){
    if(digitalRead(botoninit) == HIGH && robotState == WAITING){
      robotState = COUNTDOWN;

      digitalWrite(led1, HIGH); digitalWrite(led2, LOW); digitalWrite(led3, LOW);
      vTaskDelay(50 / portTICK_PERIOD_MS); 
      digitalWrite(led1, LOW); digitalWrite(led2, HIGH);
      vTaskDelay(50 / portTICK_PERIOD_MS); 

      fightEnabled = true;
      robotState = EXPLOSIVE;
      startFightTime = millis();
    }
    vTaskDelay(20);
  }
}

// Tarea para el control de los LEDs
void TaskLED(void *pv){
  for(;;){
    if(robotState == WAITING) {
      digitalWrite(led1, HIGH); digitalWrite(led2, LOW); digitalWrite(led3, LOW); // Rojo
    } else if(robotState == COUNTDOWN){
      vTaskDelay(50);
      digitalWrite(led1, LOW); digitalWrite(led2, HIGH); digitalWrite(led3, LOW); // Verde
    } else {
      digitalWrite(led1, LOW); digitalWrite(led2, LOW); digitalWrite(led3, HIGH); // Azul
    }
    vTaskDelay(100);
  }
}

// Setup
void setup() {
  pinMode(m1, OUTPUT); pinMode(m2, OUTPUT);
  pinMode(m3, OUTPUT); pinMode(m4, OUTPUT);

  pinMode(sdl13, INPUT); pinMode(sdl14, INPUT);  // Sólo estos dos sensores de línea
  pinMode(trigp, OUTPUT); pinMode(echop, INPUT);
  pinMode(led1, OUTPUT); pinMode(led2, OUTPUT); pinMode(led3, OUTPUT);
  pinMode(botoninit, INPUT);  // Botón de inicio

  Serial.begin(115200);  // Inicializa el puerto serial para depuración

  xTaskCreatePinnedToCore(TaskLineSensor, "linea", 2000, NULL, 3, NULL, 1);
  xTaskCreatePinnedToCore(TaskUltrasonic, "ultrasonico", 2000, NULL, 2, NULL, 1);
  xTaskCreatePinnedToCore(TaskCombat, "combate", 2000, NULL, 2, NULL, 1);
  xTaskCreatePinnedToCore(TaskStart, "arranque", 2000, NULL, 3, NULL, 0);
  xTaskCreatePinnedToCore(TaskLED, "leds", 1500, NULL, 1, NULL, 0);

  stopMotors();  // Detén los motores inicialmente
}

// Loop vacío (la lógica se maneja con FreeRTOS)
void loop(){}
