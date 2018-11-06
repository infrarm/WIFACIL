#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>

#include "DHT.h"
#define DHTTYPE DHT11   // DHT 11

// Replace with your network credentials
const char* ssid = "WIFI";
const char* password = "senai2018";
 
ESP8266WebServer server(80);   //instantiate server at port 80 (http port)

// DHT Sensor
const int DHTPin = D5;
// Initialize DHT sensor.
DHT dht(DHTPin, DHTTYPE);

// Temporary variables
static char celsiusTemp[7];
static char fahrenheitTemp[7];
static char humidityTemp[7];
 
String page = "";
int LED2 = D2;
int LED3 = D3;
int LED4 = D4;

void setup(void){
  dht.begin();
  //HTML PARA A PÁGINA A EXIBIR
  page = "<h1>ACIONAMENTO WIFI - TMECA 28</h1><p>DISPOSITIVO A &nbsp;&nbsp;&nbsp;<a href=\"LED2On\"><button>ON</button></a>&nbsp;<a href=\"LED2Off\"><button>OFF</button></a></p><p>DISPOSITIVO B &nbsp;&nbsp;&nbsp;<a href=\"LED3On\"><button>ON</button></a>&nbsp;<a href=\"LED3Off\"><button>OFF</button></a></p><p>DISPOSITIVO C &nbsp;&nbsp;&nbsp;<a href=\"LED4On\"><button>ON</button></a>&nbsp;<a href=\"LED4Off\"><button>OFF</button></a></p><table><tr><th>DISPOSITIVO</th><th>STATUS</th> <th>OPC</th></tr><tr><td>DISP. A</td><td>OFF</td> <td>[73.8]</td></tr><tr><td>DISP. B</td><td>OFF</td> <td>[23.5]</td></tr><tr><td>DISP. C</td><td>OFF</td> <td>[38.25]</td></tr></table>";
  //make the LED pin output and initially turned off
  pinMode(LED2, OUTPUT);
  pinMode(LED3, OUTPUT);
  pinMode(LED4, OUTPUT);
  
  digitalWrite(LED2, LOW);
  digitalWrite(LED3, LOW);
  digitalWrite(LED4, LOW);
  
  WiFi.disconnect(); 
  delay(1000);
  Serial.begin(115200);
  WiFi.begin(ssid, password); //begin WiFi connection
  Serial.println("");
 
  // Wait for connection
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
   
  server.on("/", [](){
    server.send(200, "text/html", page);
    leituradht();
  });

  //-----------------------------------------------
  server.on("/LED2On", [](){
    leituradht();
    server.send(200, "text/html", page);
    digitalWrite(LED2, HIGH);
    delay(1000);
  });
  server.on("/LED2Off", [](){
    leituradht();
    server.send(200, "text/html", page);
    digitalWrite(LED2, LOW);
    delay(1000); 
  });
  //-----------------------------------------------

  //-----------------------------------------------
  server.on("/LED3On", [](){
    leituradht();
    server.send(200, "text/html", page);
    digitalWrite(LED3, HIGH);
    delay(1000);
  });
  server.on("/LED3Off", [](){
    leituradht();
    server.send(200, "text/html", page);
    digitalWrite(LED3, LOW);
    delay(1000); 
  });
  //-----------------------------------------------

  //-----------------------------------------------
  server.on("/LED4On", [](){
    leituradht();
    server.send(200, "text/html", page);
    digitalWrite(LED4, HIGH);
    delay(1000);
  });
  server.on("/LED4Off", [](){
    leituradht();
    server.send(200, "text/html", page);
    digitalWrite(LED4, LOW);
    delay(1000); 
  });
  //-----------------------------------------------
  
  server.begin();
  Serial.println("Web server started!");

            //FIM VOID SETUP
}
 
void loop(void){
  server.handleClient();
}
void leituradht(){
              // Sensor readings may also be up to 2 seconds 'old' (its a very slow sensor)
            float h = dht.readHumidity();
            // Read temperature as Celsius (the default)
            float t = dht.readTemperature();
            // Read temperature as Fahrenheit (isFahrenheit = true)
            float f = dht.readTemperature(true);
            // Check if any reads failed and exit early (to try again).
            if (isnan(h) || isnan(t) || isnan(f)) {
              Serial.println("Failed to read from DHT sensor!");
              strcpy(celsiusTemp,"Failed");
              strcpy(fahrenheitTemp, "Failed");
              strcpy(humidityTemp, "Failed");         
            }
            else{
              // Computes temperature values in Celsius + Fahrenheit and Humidity
              float hic = dht.computeHeatIndex(t, h, false);       
              dtostrf(hic, 6, 2, celsiusTemp);             
              float hif = dht.computeHeatIndex(f, h);
              dtostrf(hif, 6, 2, fahrenheitTemp);         
              dtostrf(h, 6, 2, humidityTemp);
              // You can delete the following Serial.print's, it's just for debugging purposes
              Serial.print("Humidity: ");
              Serial.print(h);
              Serial.print(" %\t Temperature: ");
              Serial.print(t);
              Serial.print(" *C ");
              Serial.print(f);
              Serial.print(" *F\t Heat index: ");
              Serial.print(hic);
              Serial.print(" *C ");
              Serial.print(hif);
              Serial.print(" *F");
              Serial.print("Humidity: ");
              Serial.print(h);
              Serial.print(" %\t Temperature: ");
              Serial.print(t);
              Serial.print(" *C ");
              Serial.print(f);
              Serial.print(" *F\t Heat index: ");
              Serial.print(hic);
              Serial.print(" *C ");
              Serial.print(hif);
              Serial.println(" *F");
            }
}
