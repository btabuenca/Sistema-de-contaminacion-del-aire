
//Librerias
#include <Wire.h>
#include "ESP8266WiFi.h"  
#include "MQ135.h"          // librerias de la ESP8266
#include <DHT.h>             // librerias sesnor humedad y temperatutra
#include <DHT_U.h>   
#include <ThingerESP8266.h>    // libreria API thinger.io
#include <Adafruit_Sensor.h> // librerias del sensor de contaminacion del aire

//Puertos
#define DHTPIN D2           // Puerto sensor DHT22
#define DHTTYPE DHT22
#define MQ A0               // Puerto sensor MQ135
#define MQ2 D3              // Puerto del sensor MQ2
#define LISTEN_PORT 80      // Puerto de conexion web
#define BUZZER D4           // Puerto del Buzzer


//Inicializacion de sensores
DHT dht(DHTPIN, DHTTYPE);

//Red WIFI
const char WIFI_ssid [] = "ssId-del-wifi-a-utilizar";
const char WIFI_password [] = "contraseña-del-wifi-a-utilizar";

//Parametros de conexion con thinger.io
#define usuario "Id_usuario-thinger.io"
#define device_Id "Nombre-estacion"
#define device_credentials "credenciales-obtenidas-desde-thinger.io" // "z@HmT+c_mlH9"

MQ135 gasSensor = MQ135(MQ);

ThingerESP8266 thing(usuario, device_Id, device_credentials);

//Setup
void setup(){
  
  Serial.begin(9600);   // para que lo sensores lean a la misma velocidad
  dht.begin();          // Inicializar el sensor DHT22

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
    digitalWrite(BUZZER, digitalRead(MQ2), 400);  //El output del buzzer es el contrario al intput del sensor MQ2
    thing.handle();
}
