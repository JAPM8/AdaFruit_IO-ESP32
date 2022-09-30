// Adafruit IO Group Publish Example
//
// Adafruit invests time and resources providing this open source code.
// Please support Adafruit and open source hardware by purchasing
// products from Adafruit!
//
// Written by Todd Treece for Adafruit Industries
// Copyright (c) 2016 Adafruit Industries
// Licensed under the MIT license.
//
// All text above must be included in any redistribution.

//Programa modificado por: Javier Alejandro Pérez Marín
/************************** Configuration ***********************************/
#include <SPI.h>
#include "config.h" //Config USER y WIFI

/************************ Código *******************************/
const int slaveAPin = 15;
uint8_t temp, mov, mins, hrs, lux;
unsigned long t1 = 0; //Variable para función Millis()

// Configuración SPI
SPISettings settings(1000000, MSBFIRST, SPI_MODE1);

// Configuración de grupo de feeds
AdafruitIO_Group *up = io.group("smart-home");


void setup() {

  // Se inicia com. UART
  Serial.begin(115200);

  // Se espera a que el serial se inicialice
  while(! Serial);

  // Conexión a io.adafruit.com
  Serial.print("Conectando a Adafruit IO");
  io.connect();

  // Se espera la conexión estable
  while(io.status() < AIO_CONNECTED) {
    Serial.print(".");
    delay(500);
  }

  // Se indica la conexión exitosa
  Serial.println();
  Serial.println(io.statusText());

  SPI.begin(); //VSPI pins 
  
  t1 = millis(); //Se guarda valor inicial de función de millis()
  pinMode (slaveAPin, OUTPUT); //CS como output
}

void loop() {
  io.run(); //En cada repetición se mantiene la conexión con ADAFRUIT IO

  SPI.beginTransaction(settings);
  
  digitalWrite (slaveAPin, HIGH); //SS inactivo
  delay(10);
  digitalWrite (slaveAPin, LOW); //SS activo
  delay(10);
  lux = SPI.transfer(0); //Req = 0
  delay(10);
  
  digitalWrite (slaveAPin, HIGH); //SS inactivo
  delay(10);
  digitalWrite (slaveAPin, LOW); //SS activo
  delay(10);
  temp = SPI.transfer(1); //Req = 1
  delay(10);
 
  digitalWrite (slaveAPin, HIGH); //SS inactivo
  delay(10);
  digitalWrite (slaveAPin, LOW); //SS activo
  delay(10);
  mov = SPI.transfer(2); //Req = 2
  delay(10);

  digitalWrite (slaveAPin, HIGH); //SS inactivo
  delay(10);
  digitalWrite (slaveAPin, LOW); //SS activo
  delay(10);
  mins = SPI.transfer(3); //Req = 3
  delay(10);

  digitalWrite (slaveAPin, HIGH); //SS inactivo
  delay(10);
  digitalWrite (slaveAPin, LOW); //SS activo
  delay(10);
  hrs = SPI.transfer(4); //Req = 4
  delay(10);
  
  SPI.endTransaction();
  
  if (millis() > (t1 +2000)){ //Cada 2 segundos
    t1 = millis(); //Se actualiza valor de millis
    up->set("smart-home.temperatura", temp); //Se envía al feed smart-home.temperatura estado del sensor de temperatura
    up->set("smart-home.movimiento", mov); //Se envía al feed smart-home.movimiento estado del sensor de movimiento
    up->set("smart-home.horas", hrs); //Se envía al feed smart-home.horas estado de horas (RTC)
    up->set("smart-home.minutos", mins); //Se envía al feed smart-home.minutos estado de minutos (RTC)
    up->set("smart-home.lux", lux); //Se envía al feed smart-home.lux estado del sensor de luz (lux)
    up->save(); //Se guardan asignaciones en los feeds
  }

  Serial.print("Se envió temperatura: ");
  Serial.println(temp);
  Serial.print("Se envió movimiento: ");
  Serial.println(mov);
  Serial.print("Se envió hora: ");
  Serial.print(hrs);
  Serial.print(":");
  Serial.println(mins);
  Serial.print("Se envió lux: ");
  Serial.println(lux);

}
