#include <Servo.h>

Servo servo;

// Pines PWM para el control de velocidad de motores
const int IN1 = 3;  // Motor Izquierdo avance
const int IN2 = 5;  // Motor Izquierdo retroceso
const int IN3 = 6;  // Motor Derecho avance
const int IN4 = 11; // Motor Derecho retroceso

// Pines del sensor ultrasónico HC-SR04
const int pinTrigger = 8;
const int pinEcho = 9;

// Configuración de velocidad PWM (0 a 255)
int velocidad = 130; 

void setup() {
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);
  
  servo.attach(12);
  
  pinMode(pinTrigger, OUTPUT);
  pinMode(pinEcho, INPUT);
  
  // Posición inicial: mirando al frente
  servo.write(90);
  delay(500);
}

void loop() {
  int distanciaFrente = MedirDistanciaPrecisa();

  if (distanciaFrente < 20) {
    Stop();
    delay(300);
    
    // Retrocede un poco para despejar espacio
    Retroceder();
    delay(400);
    Stop();
    delay(300);
    
    // Escanea ambos lados usando el servomotor
    int distanciaIzquierda = MirarIzquierda();
    int distanciaDerecha = MirarDerecha();
    
    // Vuelve al frente
    servo.write(90);
    delay(300);

    // Decide la mejor ruta según el lado con ayor espacio libre
    if (distanciaIzquierda > distanciaDerecha)  {
      GirarIzquierda();
      delay(500);
    } else if (distanciaDerecha > distanciaIzquierda) 
    {
      GirarDerecha();
      delay(500);
    }
    
    Stop();
    delay(200);
  } 
  else {
    Avanzar();  
  }
}

// --- MEDICIÓN CON FILTRADO DE MEDIANA Y LECTURA PRECISA ---
int MedirDistanciaPrecisa() {
  const int MUESTRAS = 5;
  long sumaDuraciones = 0;
  int lecturasValidas = 0;

  for (int i = 0; i < MUESTRAS; i++) {
    digitalWrite(pinTrigger, LOW);
    delayMicroseconds(2);
    digitalWrite(pinTrigger, HIGH);
    delayMicroseconds(10);
    digitalWrite(pinTrigger, LOW);
    
    long duracion = pulseIn(pinEcho, HIGH, 25000); // Timeout ~4 metros
    
    if (duracion > 0) {
      sumaDuraciones += duracion;
      lecturasValidas++;
    }
    delayMicroseconds(500); // Breve pausa entre pulso y pulso
  }

  if (lecturasValidas == 0) {
    return 999; // Retorna valor alto si no hubo ecos válidos
  }

  long promedioDuracion = sumaDuraciones / lecturasValidas;
  
  // Fórmula precisa: distancia = (duración * 0.0343) / 2
  // Que equivale a: duración / 58.3
  return (int)(promedioDuracion / 58.3);
}

// --- FUNCIONES DE ESCANEO DEL SERVO ---
int MirarIzquierda() {
  servo.write(160);
  delay(400); // Tiempo para que el servo llegue a la posición
  int dist = MedirDistanciaPrecisa();
  return dist;
}

int MirarDerecha() {
  servo.write(20);
  delay(400);
  int dist = MedirDistanciaPrecisa();
  return dist;
}

// --- FUNCIONES DE MOVIMIENTO CON CONTROL PWM ---
void Avanzar() {
  analogWrite(IN1, velocidad);
  digitalWrite(IN2, LOW);
  analogWrite(IN3, velocidad);
  digitalWrite(IN4, LOW);
}

void Retroceder() {
  digitalWrite(IN1, LOW);
  analogWrite(IN2, velocidad);
  digitalWrite(IN3, LOW);
  analogWrite(IN4, velocidad);
}

void GirarDerecha() {
  digitalWrite(IN1, LOW);
  analogWrite(IN2, velocidad);
  analogWrite(IN3, velocidad);
  digitalWrite(IN4, LOW);
}

void GirarIzquierda() {
  analogWrite(IN1, velocidad);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  analogWrite(IN4, velocidad);
}

void Stop() {
  analogWrite(IN1, 0);
  analogWrite(IN2, 0);
  analogWrite(IN3, 0);
  analogWrite(IN4, 0);
}
