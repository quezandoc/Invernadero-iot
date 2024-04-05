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

  if (t >= 15 && t <= 25){  // si la temperatura es mayor a 15 y menor a 25 
    digitalWrite(LED, LOW);               // en bajo el pin 12
  }
  else {
    digitalWrite(LED, HIGH);                     // en alto el pin 12 si la temperatura es menor a 15 o mayor a 25 
    Serial.println("Alarma.......");         // envia la palabra Alarma por el puerto serial
  }
  
  if (h < 70 || h > 80){   // si la humedad es mayor a 80 o menor a 70 
    digitalWrite(LED, HIGH);               
    Serial.println("Alarma.......");         // envia la palabra Alarma por el puerto serial
  }
  else{
    digitalWrite(LED, LOW);               // en bajo el pin 12 si la humedad es menor a 80 o mayor a 70 
  }

  delay(2000); //Se espera 2 segundos para seguir leyendo //datos
}