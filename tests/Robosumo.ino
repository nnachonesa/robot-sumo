long t;
long d;
unsigned long timerOne = 0;
int M = 0;
int inicio = 0;

/* void IRAM_ATTR sensorA()
{
  digitalWrite(16, HIGH);
  delay(300);
  digitalWrite(16, LOW);
}

void IRAM_ATTR sensorB()
{
  digitalWrite(17, HIGH);
  delay(300);
  digitalWrite(17, LOW);
}

void IRAM_ATTR sensorC()
{
  digitalWrite(4, HIGH);
  digitalWrite(5, HIGH);
  delay(300);
  digitalWrite(4, LOW);
  digitalWrite(5, LOW);
} */

void setup()
{
  pinMode(4, OUTPUT);
  pinMode(16, OUTPUT);
  pinMode(17, OUTPUT);
  pinMode(5, OUTPUT);
  pinMode(18, INPUT);
  pinMode(19, INPUT);
  pinMode(21, INPUT);
  pinMode(22, INPUT);
  pinMode(23, OUTPUT);
  pinMode(13, OUTPUT);
  pinMode(27, OUTPUT);
  pinMode(26, OUTPUT);
  pinMode(25, INPUT);

  Serial.begin(9600);
  digitalWrite(23, LOW);

  /*   attachInterrupt(digitalPinToInterrupt(18), sensorA, RISING);
    attachInterrupt(digitalPinToInterrupt(19), sensorB, RISING);
    attachInterrupt(digitalPinToInterrupt(21), sensorC, RISING); */
}
void apagartodo()
{
  digitalWrite(4, LOW);
  digitalWrite(16, LOW);
  digitalWrite(5, LOW);
  digitalWrite(17, LOW);
}

void comprobacionSDL()
{
  // negro es 1, sa pata 18, sb pata 19, sc pata 21
  // motor izquierdo atras 17, adelante 5, motor derecho atras 16, adelante 4
  int sa = digitalRead(18);
  int sb = digitalRead(19);
  int sc = digitalRead(21);

  if (sa == HIGH)
  {
    apagartodo();
    digitalWrite(17, HIGH);
    digitalWrite(16, HIGH);
    delay(1000);
    digitalWrite(17, LOW);
    digitalWrite(16, LOW);
  }
  if (sb == HIGH)
  {
    apagartodo();
    digitalWrite(17, HIGH);
    digitalWrite(16, HIGH);
    delay(1000);
    digitalWrite(17, LOW);
    digitalWrite(16, LOW);
  }
  if (sc == HIGH)
  {
    apagartodo();
    digitalWrite(17, LOW);
    digitalWrite(16, LOW);
    delay(1000);
    digitalWrite(17, HIGH);
    digitalWrite(16, HIGH);
  }
}

void loop()
{
  digitalWrite(23, HIGH);
  delayMicroseconds(10);
  digitalWrite(23, LOW);
  t = pulseIn(22, HIGH);
  d = t / 59;
  if (digitalRead(25) == HIGH)
  { // activacion inicial
    inicio = 1;
    M = 1;
    digitalWrite(13, HIGH);
    digitalWrite(26, LOW);
    digitalWrite(27, LOW);
    timerOne = millis();
    delay(3000);
  }
  if (M == 1)
  {
    if (d <= 40)
    { // escaneo avanzar o rotar
      digitalWrite(4, LOW);
      digitalWrite(17, LOW);
      delay(200);
      digitalWrite(4, HIGH);
      digitalWrite(5, HIGH);
    }
    else
    {
      digitalWrite(4, HIGH);
      digitalWrite(17, HIGH);
    }
  }
  if (inicio == 1)
  { // tiempo agotado
    if (millis() - timerOne >= 120000)
    {
      M = 0;
      inicio = 0;

      digitalWrite(4, LOW);
      digitalWrite(16, LOW);
      digitalWrite(5, LOW);
      digitalWrite(17, LOW); // apagar los motores

      digitalWrite(27, LOW);
      digitalWrite(13, LOW);
      digitalWrite(26, HIGH);
      delay(1000);
      digitalWrite(26, LOW);
      delay(1000);
      digitalWrite(26, HIGH);
      delay(1000);
      digitalWrite(26, LOW);
      delay(1000);
      digitalWrite(26, HIGH);
      delay(1000);
      digitalWrite(26, LOW);
      digitalWrite(27, HIGH);
    }
  }
}
