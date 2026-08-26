# UNObot 
Este proyecto consiste en armar un robot basico con Arduino Uno capaz de evitar obstaculos con sensor ultrasonico y un motor driver L298N que generalmente es el mas usado para proyectos educativos, aunque reconozco que actualmente hay drivers mas eficientes. Este es unos de los proyectos mas típicos en el ambito de la robótica educativa 

Para este proyecto se va a necesitar un chasis 2wd que se puede conseguir facilmente por internet.

![](https://www.teslaelectronic.com.pe/wp-content/uploads/2023/03/2WD-Car4.png)

- Un Arduino UNO que puede ser original o clon, este es el cerebro del proyecto y donde se va a ejecutar el programa del robot

![](https://hellbot.xyz/wp-content/uploads/2019/12/Uno.jpg)

- Un portapilas x2 en serie de tipo 18650 

![](https://hipercom.com.ar/wp-content/uploads/2021/10/D_940974-MLA40584109550_012020-F.jpg)

- 2 Baterias 18650 (lo ideal es que sean Samsung marca original modelo INR-30Q, que ofrecen una autonomía mayor)

![](https://encrypted-tbn0.gstatic.com/images?q=tbn:ANd9GcRXTqGe_s96ATthnhkoJ9-iLSO6T4XmZ-ltSTGFGlDwj-Y5OQTuBEnT_gw&s=10)

- Un módulo l298n para controlar lo motores de corriente directa que es bastante accesible

![](https://naylampmechatronics.com/148-superlarge_default/driver-puente-h-l298n.jpg)

- Un servo sg90 para mover el sensor ultrasonico 

![](https://naylampmechatronics.com/774-superlarge_default/servo-sg90-1kg.jpg)

- Un sensor ultrasonico HC-SR04 que va a permitir la detección de objetos
![](https://www.sparkfun.com/media/catalog/product/cache/f3020b7489dcfc4d1d147cf4dad07b7f/1/5/15569-Ultrasonic_Distance_Sensor_-_HC-SR04-01a.jpg)

- Cables dupont para el cableado

![](https://www.electronicamendoza.com.ar/content/images/thumbs/66803c693e04d60001164083_cable-macho-macho-20cm-cable-dupont-para-arduino-protoboard_600.jpeg)

y por ultimo un protoboard para facilitar las conexiones

![](https://dcdn-us.mitiendanube.com/stores/648/727/products/oky0005-breadboard-11-6942049fe8d566d09915287576013319-640-0.webp)



# Comenzando

Para comenzar tenemos que hacer las conexiones necesarias para que el funcionamiento del robot sea el optimo, yo personalmente use fritzing para hacer el diagramado mas fácil ya que tiene la funcionalidad de agregar componentes personalizados al circuito.

<img width="3590" height="1985" alt="UNObot diagram" src="https://github.com/user-attachments/assets/25c66bd7-3b08-4f3e-905c-4052a4af8cda" />

<h1>Como programarlo ?</h1>

Para programar el robot vamos a necesitar un entorno de desarrollo basado en Arduino por eso vamos a utilizar el mas conocido que es el Arduino IDE, les dejo el link: https://docs.arduino.cc/software/ide/ 

<img width="1919" height="906" alt="image" src="https://github.com/user-attachments/assets/52a7091c-877f-4b2e-8aad-cabd67757b62" />

<h1>Configuración del entorno</h1>

Una vez que tengamos instalado el IDE lo abrimos y procedemos a configurar la placa Arduino UNO 

<img width="1916" height="210" alt="image" src="https://github.com/user-attachments/assets/95ee1423-7b08-405f-b0b2-532618c1cc79" />
<img width="582" height="415" alt="image" src="https://github.com/user-attachments/assets/613d3333-919a-4acc-8274-85e74fd6ce34" />

En caso de que no aparezca el arduino en algun puerto serie COM utilizaremos un programa adicional para instalar el driver que es necesario en caso de que el arduino sea clon  https://www.geekfactory.mx/tutoriales-arduino/driver-ch340-para-arduino-chinos-o-genericos/?srsltid=AfmBOorHDHxkRfxt2n8idLw8O_7kmCPBaavJfe-5GAWVcD_2bNHU14v9 
<img width="494" height="328" alt="image" src="https://github.com/user-attachments/assets/07321ca3-f296-418f-9cf9-1e8d992b66da" />

<h1>Código</h1>
Para comenzar con el código primero definiremos las librerias y variables que vamos a necesitar

```cpp
#include <Servo.h>

const int IN1 = 3;  // Motor Izquierdo avance
const int IN2 = 5;  // Motor Izquierdo retroceso
const int IN3 = 6;  // Motor Derecho avance
const int IN4 = 11; // Motor Derecho retroceso
```
Vamos a incluir la libreria Servo.h que es una libreria que se utiliza para controlar servomotores con arduino, luego definieremos unas variables constantes enteras que van a hacer referencia a cada pin de entrada de los motores del l298n IN1,IN2,IN3,IN4, lo que nos va a permnitir cambiar los estados logicos de los motores. 

```cpp

const int pinTrigger = 8; 
const int pinEcho = 9;

int velocidad = 130;
```
Definimos el pin Trigger que es el disparador de la onda sonica que va a rebotar con el objeto y el pin echo que va a ser el receptor que va a recibir la onda previamente disparada, luego declaramos una variable para establecer la velocidad por defecto del robot que es 130 hasta 255
```cpp

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

```
La funcion void setup{} establece la configuracion inicial de los componentes como los pines de los motores por ejemplo cuando hacemos un pinMode(IN1, OUTPUT) le estamos diciendo al Arduino que configure ese pin y lo establezca como una salida, en el caso del servo, usando servo.attach(12), estamos estableciendo el pin 12 del Arduino para conectar el servomotor, en el caso de los pines del sensor ultrasonico establecemos pinMode(pinTrigger, OUTPUT) para que el Arduino sepa que mandamos una señal al exterior, que es el disparo del trigger y pinMode(pinEcho, INPUT) para que el receptor del sensor reciba la señal. Posteriormente le decimos al servomotor mediante servo.write(90) que mueva el eje del servomotor en un eje de 90° grados, teniendo en cuenta de que en este caso el servomotor que estamos usando es el modelo sg90 lo cual el eje maximo de rotación es de 180°

```cpp
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

```
Vamos a definir una función de tipo entera para medir la distancia del sensor ultrasonico, en la cual establecemos una variable para almacenar los disparos del trigger y otras dos variables para la clasificación de valores del sensor y para validar esas muestras, luego en el bucle for hacemos el proceso de disparar el trigger, con intervalos delayMicroseconds(2) y delayMicroseconds(10) en microsegundos para realizar el procedimiento, después almacenamos la duración del procedimiento al mismo tiempo que damos la orden de prender el pin Echo que es el receptor que recibe la onda rebotada del disparo. Finalmente validamos la duración para posteriormente clasificarla, evitar lecturas erroneas del sensor y devolver el calculo de la duración que tarda el sensor en disparar la onda ultrasonica y recibirla.

```cpp
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
```
Como paso final definiremos las funciones encargadas de controlar los motores y hacer que el robot se mueva.
