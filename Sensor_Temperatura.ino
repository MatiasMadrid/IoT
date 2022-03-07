#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include "credenciales.h"

const char* servidor_mqtt = "192.168.0.11"; //ip de las raspberry
int puerto = 1883;

WiFiClient clientNode;
PubSubClient client (clientNode); 

//-------------------------variables y pines--------------
#define sensorTemp A0
#define led1 D2
#define led2 D8
int valor;

String _topic;
String _payload;
//---------------------------------------------------------

void configuracion_wifi() //funcion para conectar al wifi
{
  delay(10);
  WiFi.begin(ssid, password); //se inicia la conexion
  while( WiFi.status() != WL_CONNECTED)
  {
    delay(500); //se reintenta establecer conexion 
  }
}

void reconnect() //funcion para establecer conexion con el broker mqtt
{
  while(!client.connected())
  {
    Serial.println("Intentando conexion MQTT");
    if (client.connect("NodeMCUClient"))
    {
      Serial.println("Conectado!");
      client.subscribe("casa/habitacion/led2");
    }
    else
    {
      Serial.print("Falla, Estado : ");
      Serial.print(client.state());
      Serial.println("Intentado conexion nuevamente en 5 segundos...");
      delay(5000);
    }
    
  }
}

//---------------funcion callback de suscripcion--------------
void callback(char* topic, byte* payload, unsigned int length)
{
  String conc_payload;
  for (int x=0; x<length; x++) //guarda en conc_playload el mensaje
  {
    conc_payload += (char)payload[x];
  }
  _topic = topic;
  _payload = conc_payload;
}

void setup() {
  Serial.begin( 115200 );
  pinMode(sensorTemp,INPUT);
  pinMode(led1, OUTPUT);
  pinMode(led2, OUTPUT);
  configuracion_wifi();
  client.setServer( servidor_mqtt, puerto );
  client.setCallback(callback);
}

void loop() {

  if (!client.connected())
  {
    reconnect();
  }
  client.loop(); //para mantener la conexion con el broker

  valor = analogRead(sensorTemp);
  client.publish("casa/habitacion/temperatura", String(valor).c_str());
  if (valor < 200) //apaga o prende el led dependiendo del sensor
  {
    digitalWrite(led1, HIGH);
  }
  else
  {
    digitalWrite(led1, LOW);
  }

  if (_payload == "on") //apaga o prende el led por interruptor
  {
    digitalWrite(led2, HIGH);
  }
  if (_payload == "off")
  {
    digitalWrite(led2, LOW);
  }
  
  delay(1000);
}
