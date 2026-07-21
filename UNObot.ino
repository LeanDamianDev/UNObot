/* Robot UNObot 
  Componentes Usados:
  - Arduino Uno
  - Portapilas 18650 x2 serie
  - x2 pilas 18650
  - modulo driver l298n
  - 1 servo sg90
  - 1 sensor ultrasonico hc-sr04
*/
#include <Servo.h>

// Pines Motores L298N
const int IN1 = 2; const int IN2 = 3;
const int IN3 = 4; const int IN4 = 5; 

// Pines Ultrasonico HC-SR04
const int pinTrigger = 8;
const int pinEcho = 9;

// Servo
Servo miServo;
const int pinServo = 4;

// Configuración de distancia límite
const int distanciaObstaculo = 20; // cm

void setup() {
  // Configurar motores
  pinMode(IN1, OUTPUT); pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT); pinMode(IN4, OUTPUT);
  
  // Configurar ultrasonico
  pinMode(pinTrigger, OUTPUT);
  pinMode(pinEcho, INPUT);
  digitalWrite(pinTrigger, LOW);
  // Configurar servo
  miServo.attach(pinServo);
  miServo.write(90); 
  
  delay(1000); 
}
// Función para obtener la distancia 
int obtenerDistancia() {
  digitalWrite(pinTrigger, HIGH); // dispara el trigger
  delayMicroseconds(10);
  digitalWrite(pinTrigger, LOW);
  
  long duracion = pulseIn(pinEcho, HIGH, 30000); // escucha echo
  if (duracion == 0) return 999; 
  
  return duracion / 59; // conversión de la distancia obtenida 
}
void loop() {
  int distanciaAlFrente = obtenerDistancia();

  // Condición para reaccionar frente un obstaculo dependiendo de su distacia
  if (distanciaAlFrente <= distanciaObstaculo && distanciaAlFrente > 0) {
    quedarseQuieto();
    delay(500);
    miServo.write(0);
    delay(1000);
    miServo.write(90);
    delay(1000);
    miServo.write(180);
    moverAtras();
    delay(600); 
    quedarseQuieto();
    delay(300);

    girarDerecha();
    delay(500);
    quedarseQuieto();
    delay(300);
   } else {
    
   moverAdelante();
  }
  delay(50); 
}



void moverAdelante() {

  digitalWrite(IN1, HIGH); digitalWrite(IN2, LOW);  
  digitalWrite(IN3, HIGH); digitalWrite(IN4, LOW); 
}   

void moverAtras() {
  digitalWrite(IN1, LOW);  digitalWrite(IN2, HIGH); 
  digitalWrite(IN3, LOW); digitalWrite(IN4, HIGH);  
}

void girarDerecha() {

  digitalWrite(IN1, HIGH); digitalWrite(IN2, LOW);  
  digitalWrite(IN3, LOW); digitalWrite(IN4, HIGH);  
}

void quedarseQuieto() {
  digitalWrite(IN1, LOW); digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW); digitalWrite(IN4, LOW);
}
