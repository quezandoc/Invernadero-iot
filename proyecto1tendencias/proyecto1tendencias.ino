//Sensor de temperatura y humedad DHT11
#include "DHT.h" //cargamos la librería DHT
#define DHTPIN 2 //Seleccionamos el pin en el que se //conectará el sensor
#define LED 12 //Seleccionamos el pin en el que se //conectará el led
#define DHTTYPE DHT11 //Se selecciona el DHT11 que se va usar
DHT dht(DHTPIN, DHTTYPE); //Se inicia una variable que será usada por Arduino para comunicarse con el sensor

void setup() {
  Serial.begin(9600); //Se inicia la comunicación serial
  pinMode(LED, OUTPUT);  // Define el pin 12 como salida
  dht.begin(); //Inicio del sensor
}

void loop() {
  float h = dht.readHumidity(); //Se lee la humedad
  float t = dht.readTemperature(); //Se lee la temperatura
  //Se imprimen las variables
  Serial.println("Humedad: ");
  Serial.println(h);
  Serial.println("Temperatura: ");
  Serial.println(t);
 
  // Caso de tomates
  if ((t < 16 || t > 27) || (h < 60 || h > 70)) {
    Serial.print("Alarma--------------- ");
    digitalWrite(LED, HIGH); // Enciende LED para alarma
  } else {
    digitalWrite(LED, LOW); // Apaga LED si no hay alarma
  }

// Caso de lechugas
  if ((t < 10 || t > 24) || (h < 50 || h > 70)) {
    Serial.print("Alarma--------------- ");
    digitalWrite(LED, HIGH); // Enciende LED para alarma
  } else {
    digitalWrite(LED, LOW); // Apaga LED si no hay alarma
  }

// Caso de zapallo
  if ((t < 18 || t > 32) || (h < 60 || h > 80)) {
    Serial.print("Alarma--------------- ");
    digitalWrite(LED, HIGH); // Enciende LED para alarma
  } else {
    digitalWrite(LED, LOW); // Apaga LED si no hay alarma
  }

// Caso de papas
  if ((t < 10 || t > 22) || (h < 80 || h > 90)) {
    Serial.print("Alarma--------------- ");
    digitalWrite(LED, HIGH); // Enciende LED para alarma
  } else {
    digitalWrite(LED, LOW); // Apaga LED si no hay alarma
  }

  delay(2000); //Se espera 2 segundos para seguir leyendo //datos
}