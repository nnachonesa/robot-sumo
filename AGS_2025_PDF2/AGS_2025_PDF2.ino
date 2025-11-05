#include <Arduino.h>

// Pines
const int m1 = 4, m2 = 5, m3 = 6, m4 = 7; // L298N señales
const int sdl11 = 13, sdl12 = 14, sdl13 = 15, sdl14 = 16;
const int trigp = 34, echop = 17;
const int led1 = 18, led2 = 19, led3 = 21;
const int botoninit = 35;

float dis = 1000;
unsigned long startFightTime = 0; // para el bien de los ojos del los miembros del grupo, variables las cuales sus nombres si son descriptivas
bool fightEnabled = false;

enum State {
  WAITING,
  COUNTDOWN,
  EXPLOSIVE,
  SAFE_FIGHT
}; // lo use para que sea legible
State robotState = WAITING;

// funciones motores (L298N full power se llama cuando es al mango)
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

// tarea del multi: sensores de linea (ahora en priori max)
void TaskLineSensor(void *pv){
  for(;;){
    bool linea = (digitalRead(sdl11)==LOW || digitalRead(sdl12)==LOW ||
                  digitalRead(sdl13)==LOW || digitalRead(sdl14)==LOW);

    if(linea && fightEnabled){
      backward();
      vTaskDelay(300/portTICK_PERIOD_MS);
      turnRight();
      vTaskDelay(300/portTICK_PERIOD_MS);
    }

    vTaskDelay(1);
  }
}

// tarea multisonido
void TaskUltrasonic(void *pv){
  for(;;){
    digitalWrite(trigp, LOW);
    delayMicroseconds(2);
    digitalWrite(trigp, HIGH);
    delayMicroseconds(10);
    digitalWrite(trigp, LOW);

    unsigned long duration = pulseIn(echop, HIGH, 20000);
    if(duration > 0) dis = duration * 0.034 / 2;
    else dis = 200; // nada detectado

    vTaskDelay(30 / portTICK_PERIOD_MS);
  }
}

// tarea logica de pelea
void TaskCombat(void *pv){
  for(;;){
    if(!fightEnabled){
      stopMotors();
      vTaskDelay(10);
      continue;
    }

    //  2.5s inicial
    if(robotState == EXPLOSIVE){
      forward();
      if(millis() - startFightTime >= 2500){
        robotState = SAFE_FIGHT;
      }
    }
    else if(robotState == SAFE_FIGHT){
      if(dis <= 30){
        forward(); // atacar
      } else {
        turnRight(); // buscar
      }
    }

    vTaskDelay(10);
  }
}

// start del coso de inicio oficial sumo
void TaskStart(void *pv){
  for(;;){
    if(digitalRead(botoninit) == HIGH && robotState == WAITING){
      robotState = COUNTDOWN;

      digitalWrite(led1, HIGH); digitalWrite(led2, LOW); digitalWrite(led3, LOW);
      vTaskDelay(2000 / portTICK_PERIOD_MS); // 2s
      digitalWrite(led1, LOW); digitalWrite(led2, HIGH);
      vTaskDelay(2000 / portTICK_PERIOD_MS); // otros 2s

      fightEnabled = true;
      robotState = EXPLOSIVE;
      startFightTime = millis();
    }
    vTaskDelay(20);
  }
}

// tarea leds estado
void TaskLED(void *pv){
  for(;;){
    if(robotState == WAITING) {
      digitalWrite(led1, HIGH); digitalWrite(led2, LOW); digitalWrite(led3, LOW);
    } else if(robotState == COUNTDOWN){
      digitalWrite(led1, LOW); digitalWrite(led2, HIGH); digitalWrite(led3, LOW);
    } else {
      digitalWrite(led1, LOW); digitalWrite(led2, LOW); digitalWrite(led3, HIGH);
    }
    vTaskDelay(100);
  }
}

// setup
void setup() {
  pinMode(m1, OUTPUT); pinMode(m2, OUTPUT);
  pinMode(m3, OUTPUT); pinMode(m4, OUTPUT);

  pinMode(sdl11, INPUT); pinMode(sdl12, INPUT);
  pinMode(sdl13, INPUT); pinMode(sdl14, INPUT);

  pinMode(trigp, OUTPUT); pinMode(echop, INPUT);
  pinMode(led1, OUTPUT); pinMode(led2, OUTPUT); pinMode(led3, OUTPUT);
  pinMode(botoninit, INPUT);

  Serial.begin(115200);

  xTaskCreatePinnedToCore(TaskLineSensor, "linea", 2000, NULL, 3, NULL, 1);
  xTaskCreatePinnedToCore(TaskUltrasonic, "ultrasonico", 2000, NULL, 2, NULL, 1);
  xTaskCreatePinnedToCore(TaskCombat, "combate", 2000, NULL, 2, NULL, 1);
  xTaskCreatePinnedToCore(TaskStart, "arranque", 2000, NULL, 3, NULL, 0);
  xTaskCreatePinnedToCore(TaskLED, "leds", 1500, NULL, 1, NULL, 0);
  // xTaskCreatePinnedToCore(fn, ndebug, size_stack, params para la tarea, prioridad, handle, core)
  // size = "words"x4bytes=x p/stack
  stopMotors();
}

// loop vacio (freertos controla todo entonces no es necesario usar loop)
void loop(){}
