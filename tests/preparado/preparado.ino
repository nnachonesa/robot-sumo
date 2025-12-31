const int m1 = 4, m2 = 5,
/*
  m1=motor 1=motor izquierdo
  m2=motor 2=motor derecho
*/
sdl1=13, sdl2=14, sdl3=15, sdl4=16, trigp= 34,
// para el echo
 echop = 17,
 led1 = 18,
 led2 = 19,
 led3 = 21,
 botoninit = 35;
/*
  sdl1=13=abajo derecha}
  sdl2=12=arriba izqeuoierr
  sdl3=11=arriba der
  sdl4=10=abajo izq*/
long time;
float distance;
bool inicial = false;

void setup() {
  pinMode(butoninit, INPUT);
  pinMode(led1, OUTPUT);
  pinMode(led2, OUTPUT);
  pinMode(led3, OUTPUT);
  pinMode(sdl1, INPUT);
  pinMode(sdl2, INPUT);
  pinMode(sdl3, INPUT);
    pinMode(sdl4, INPUT);
    pinMode(trigp, OUTPUT);
    pinMode(echop, INPUT);
    pinMode(m1, OUTPUT);
    pinMode(m2, OUTPUT);
    Serial.begin(9600);
}
int leds() {
  /**
  amarillo espera 0
  verde comopetencia iniciada 1
  rojo competencia terminada 2
  */
  int temp = 0;
  switch(temp) {
    case 0: {
      digitalWrite(led1, HIGH);
      digitalWrite(led2, LOW);
      digitalWrite(led3, LOW);
    };break;
    case 1: {
      digitalWrite(led1, LOW);
      digitalWrite(led2, HIGH);
      digitalWrite(led3, LOW);
    };break;
    case 2: {
      /**
      agregar funcion de todas las interrupciones de los pines
      */
      digitalWrite(led1, LOW);
      digitalWrite(led2, LOW);
      digitalWrite(led3, HIGH);
      delay(1000);
      digitalWrite(led3, LOW);
      delay(1000);
      digitalWrite(led3, HIGH);
      delay(1000);
      digitalWrite(led3, LOW);
      delay(1000);
      digitalWrite(led3, HIGH);
      delay(1000);
      digitalWrite(led1, HIGH);
      digitalWrite(led2, LOW);
      digitalWrite(led3, LOW);
    };
  }
}

void avanzar() {
  int lecturadig1 =  digitalRead(sdl1),
  lecturadig2 = digitalRead(sld12);
  if(lecutradig1 && lecturadig2) {
    /*
    * dependiendo de si cero es negro o blanco es 1
    * se debe de agregar el not a la condicion
    */
    analogWrite(m2, 100); 
    analogWrite(m1, 100);
  } 
}

void frenar() {
    analogWrite(m2, 0); 
    analogWrite(m1, 0);
}

void retroceder() {
  int lecturadig1 =  digitalRead(sdl3),
  // detachInterrupt(digitalPinToInterrupt(sdl3))
  lecturadig2 = digitalRead(sld14);
  if(lecutradig1 && lecturadig2) {
    /*
    * dependiendo de si cero es negro o blanco es 1
    * se debe de agregar el not a la condicion
    */
    analogWrite(m2, -100); 
    analogWrite(m1, -100);
  }
}

void girar(bool dir) {
  // izquierda es cero, derecha es uno
  int l1 = digitalRead(sdl1), 
    l2 = digitalRead(sdl2),
    l3 = digitalRead(sdl3),
    l4 = digitalRead(sdl4);   
  if(dir) {
  // girar derecha 
    if(sdl1 && sdl2 && sdl3) {
      analogWrite(m1, -100);
        analogWrite(m2, 100);
    }
  } else {
  // girar izquierda
    if(sdl4 && sdl2 && sdl3) {
      analogWrite(m1, 100);
        analogWrite(m2, -100);
    }
  }
}

void inicio() {
    if(inicial) inicial=false;
    else inicial=true;
  
  switch(inicial) {
    case true: {

      //primera condicion para que arranque y tire true para arrancar el while
      return true;
    };break;
    case false: {
      // segunda condicion para que tire false y detenga el while
      return false;
    };break;
  }
}

void loop() {
  while(bool()) {
  // funca del robot
  }
}