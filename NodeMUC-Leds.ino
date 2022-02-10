#include <ESP8266WiFi.h>

//Se declaran los dos leds conectados a las terminales de la placa como variables
#define LED1 D1
#define LED2 D2

//Se definen las variables de encendido/apagado
int value1 = LOW;
int value2 = LOW;

//Se declaran variables correspondientes a la red wifi
const char* ssid = "NombreWifi";
const char* password = "ContraseñaWifi";


WiFiServer server(80);

void setup() {
  //Se definen los leds
  pinMode(LED1, OUTPUT);
  pinMode(LED2, OUTPUT);
  Serial.print("Conectando a  Red WiFi ");
  Serial.begin(115200);
  
  
  Serial.print("Conectando a  Red WiFi ");
  Serial.println(ssid); //Muestra nombre de red

  WiFi.begin(ssid, password);//Inicia la conexion a la red

   while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }
  
  Serial.println("\nConectado al WiFi ");
  Serial.println(ssid);

  server.begin();//Se inicia el servidor
  Serial.println("Servidor iniciado");
  
  Serial.println("Direccion: ");
  Serial.println("http://");
  Serial.println(WiFi.localIP());
  Serial.println("\n");

}

void loop() {
  WiFiClient client = server.available(); //objeto cliente
  if (!client) //espera que un cliente se conecte al servidor
  { 
    return;
  }

  Serial.println("Cliente conectado");

  while (!client.available()) //espera peticion del cliente
  {
    delay(1); 
  }

  
  // escucha clientes entrantes
  //WiFiClient client = server.available();

  //if (client) {

    //if (client.connected()) {
    //  Serial.println("Cliente conectado");
    //}

    // cierra la conexion:
  //}

  String request = client.readStringUntil('\r');
  if (request.indexOf("/favicon") == -1){
    Serial.println("Nuevo cliente");
    Serial.print(request);
  }
  client.flush();



  if (request.indexOf("/LED1=ON") != -1){ //prende el led
    digitalWrite(LED1, HIGH);
    value1 = HIGH;
  }

  if (request.indexOf("/LED1=OFF") != -1){ //apaga el led
    digitalWrite(LED1, LOW);
    value1 = LOW;
  }
  
  if (request.indexOf("/LED2=ON") != -1){ //prende el led
    digitalWrite(LED2, HIGH);
    value2 = HIGH;
  }

  if (request.indexOf("/LED2=OFF") != -1){ //apaga el led
    digitalWrite(LED2, LOW);
    value2 = LOW;
  }

/*------------------Se crea la pagina html---------------------*/
 client.println("HTTP/1.1 200 OK");
 client.println("Content-Type: text/html");
 client.println("");
 client.println("<!DOCTYPE HTML>");
 client.println("<html>");

  client.print("LED 1 is: ");

 if(value1 == HIGH)
 {
 client.print("ON");
 }
 else
 {
 client.print("OFF");
 }
 
 client.println("<br><br>");
 client.println("<a href=\"/LED1=ON\"\"><button>ON</button></a>");
 client.println("<a href=\"/LED1=OFF\"\"><button>OFF</button></a><br />");
 client.println("</html>");

 client.print("LED 2 is: ");
 
 if(value2 == HIGH)
 {
 client.print("ON");
 }
 else
 {
 client.print("OFF");
 }

 client.println("<br><br>");
 client.println("<a href=\"/LED2=ON\"\"><button>ON</button></a>");
 client.println("<a href=\"/LED2=OFF\"\"><button>OFF</button></a><br />");
 client.println("</html>");

 delay(1);
 //client.stop();
 Serial.println("Client desconectado");
 Serial.println("");
  
}
