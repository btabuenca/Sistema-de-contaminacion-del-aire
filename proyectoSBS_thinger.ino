
//Libs

#include <Wire.h>
#//include <LCD.h>
#include "ESP8266WiFi.h"  
#include "MQ135.h"          // librerias de la ESP8266
#include <DHT.h>             // librerias sesnor humedad y temperatutra
#include <DHT_U.h>   
#include <ThingerESP8266.h>    // libreria API thinger.io
#include <Adafruit_Sensor.h> // librerias del sensor de contaminacion del aire

//Ports
#define DHTPIN D2           // Puerto sensor DHT22
#define DHTTYPE DHT22
#define MQ A0               // Puerto sensor MQ135
#define MQ2 D3              // Puerto del sensor MQ2
#define LISTEN_PORT 80      // Puerto de conexion web
#define 


//Ini sensores
DHT dht(DHTPIN, DHTTYPE);

//Red WIFI
const char WIFI_ssid [] = "KiZinho";
const char WIFI_password [] = "dubistjetztoben";

//Parametros de conexion con thinger.io
#define usuario "jmveleztorres"
#define device_Id "Lichterfelde_Berlin"
#define device_credentials "955KyqJuP&m7" // "z@HmT+c_mlH9"

MQ135 gasSensor = MQ135(MQ);

ThingerESP8266 thing(usuario, device_Id, device_credentials);

//Setup
void setup(){
  
  Serial.begin(9600);   // para que lo sensores lean a la misma velocidad
  dht.begin();          // Inicializar el sensor


  //Conexion wifi con la API
  thing.add_wifi(WIFI_ssid, WIFI_password);
  
  // Inicializar la lectura de datos con la API
  thing[device_Id] >> [](pson& out){
  out["Temperatura"] = dht.readTemperature();
  out["Humedad"] = dht.readHumidity();
  out["Contaminacion_Aire"] = gasSensor.getPPM();
  out["Presencia de metano/propano"] = digitalRead(MQ2);
  };
}

void loop(){

    thing.handle();
}