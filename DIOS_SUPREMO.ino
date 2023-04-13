#include "BluetoothSerial.h"
#include <Wire.h>
BluetoothSerial SerialBT;
// Constantes y variables para sensores
const int pinIRIzquierdo = ; //Pines análogos infrarrojos
const int pinIRDerecho = ;
const int pinIRFrontal = ;
const int pinSensorRanurado1 = ;
const int pinSensorRanurado2 = ;
const int motorIzquierdoA = ;
const int motorIzquierdoB = ;
const int motorDerechoA = ;
const int motorDerechoB = ;
int valorIRIzquierdo;
int valorIRDerecho;
int valorIRFrontal;
int valorRanurado1;
int valorRanurado2;
void leerSensores();
void enviarDatos();
void recibirComandos();
void ejecutarMovimientos(char comando);

void setup() {
  Serial.begin(115200);
  SerialBT.begin("Micromouse");
  
  pinMode(pinIRIzquierdo, INPUT);
  pinMode(pinIRDerecho, INPUT);
  pinMode(pinIRFrontal, INPUT);
  pinMode(pinSensorRanurado1, INPUT);
  pinMode(pinSensorRanurado2, INPUT);

  pinMode(motorIzquierdoA, OUTPUT);
  pinMode(motorIzquierdoB, OUTPUT);
  pinMode(motorDerechoA, OUTPUT);
  pinMode(motorDerechoB, OUTPUT);

  digitalWrite(motorIzquierdoA, LOW);
  digitalWrite(motorIzquierdoB, LOW);
  digitalWrite(motorDerechoA, LOW);
  digitalWrite(motorDerechoB, LOW);
  Serial.println("Dispositivo Bluetooth iniciado");
}

void loop() {
  leerSensores();
  enviarDatos();
  recibirComandos();
}

void leerSensores() {
  // Leer el sensor infrarrojo izquierdo
  valorIRIzquierdo = analogRead(pinIRIzquierdo);
  // Leer el sensor infrarrojo derecho
  valorIRDerecho = analogRead(pinIRDerecho);
  // Leer el sensor infrarrojo frontal
  valorIRFrontal = analogRead(pinIRFrontal);
  // Leer el primer sensor ranurado (odometría)
  valorRanurado1 = digitalRead(pinSensorRanurado1);
  // Leer el segundo sensor ranurado (odometría)
  valorRanurado2 = digitalRead(pinSensorRanurado2);
}

void enviarDatos() {
  // Envía el valor del sensor IR izquierdo
  SerialBT.print("IRL,");
  SerialBT.print(valorIRIzquierdo);
  SerialBT.print(",");
  // Envía el valor del sensor IR derecho
  SerialBT.print("IRD,");
  SerialBT.print(valorIRDerecho);
  SerialBT.print(",");
  // Envía el valor del sensor IR frontal
  SerialBT.print("IRF,");
  SerialBT.print(valorIRFrontal);
  SerialBT.print(",");
  // Envía el valor del primer sensor ranurado
  SerialBT.print("SR1,");
  SerialBT.print(valorRanurado1);
  SerialBT.print(",");
  // Envía el valor del segundo sensor ranurado
  SerialBT.print("SR2,");
  SerialBT.print(valorRanurado2);
  // Indica el final de la línea de datos
  SerialBT.println();
}

void recibirComandos() {
  if (SerialBT.available()) {
    char comando = SerialBT.read();
    ejecutarMovimientos(comando);
  }
}

void ejecutarMovimientos(char comando) {
  // Detener los motores antes de cambiar de dirección
  digitalWrite(motorIzquierdoA, LOW);
  digitalWrite(motorIzquierdoB, LOW);
  digitalWrite(motorDerechoA, LOW);
  digitalWrite(motorDerechoB, LOW);

  switch (comando) {
    case 'A': // Avanzar
      digitalWrite(motorIzquierdoA, HIGH);
      digitalWrite(motorIzquierdoB, LOW);
      digitalWrite(motorDerechoA, HIGH);
      digitalWrite(motorDerechoB, LOW);
      break;
    case 'R': // Retroceder
      digitalWrite(motorIzquierdoA, LOW);
      digitalWrite(motorIzquierdoB, HIGH);
      digitalWrite(motorDerechoA, LOW);
      digitalWrite(motorDerechoB, HIGH);
      break;
    case 'L': // Girar a la izquierda
      digitalWrite(motorIzquierdoA, LOW);
      digitalWrite(motorIzquierdoB, HIGH);
      digitalWrite(motorDerechoA, HIGH);
      digitalWrite(motorDerechoB, LOW);
      break;
    case 'D': // Girar a la derecha
      digitalWrite(motorIzquierdoA, HIGH);
      digitalWrite(motorIzquierdoB, LOW);
      digitalWrite(motorDerechoA, LOW);
      digitalWrite(motorDerechoB, HIGH);
      break;
    default:
      // Si se recibe un comando no válido, no hacer nada
      break;
  }
}
