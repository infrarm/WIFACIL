#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>

//IRSEND
#include <IRremoteESP8266.h>
#include <IRsend.h>

const uint16_t kIrLed = 4;  // ESP8266 GPIO pin to use. Recommended: 4 (D2).
IRsend irsend(kIrLed);  // Set the GPIO to be used to sending the message.

uint16_t LIGA[199] = {4456, 4320,  596, 1602,  568, 520,  570, 1606,  568, 1602,  568, 522,  570, 520,  568, 1602,  568, 524,  570, 522,  570, 1604,  570, 520,  570, 522,  570, 1602,  570, 1602,  568, 520,  570, 1606,  572, 520,  570, 522,  570, 1604,  570, 1600,  570, 1600,  570, 1602,  570, 1600,  570, 1602,  570, 1608,  570, 1600,  570, 520,  570, 520,  570, 520,  570, 520,  570, 520,  572, 520,  570, 522,  570, 1602,  570, 520,  570, 1606,  570, 520,  570, 520,  570, 520,  570, 522,  570, 1606,  572, 518,  570, 1606,  568, 520,  570, 1606,  570, 1600,  570, 1600,  570, 1602,  634, 5094,  4446, 4312,  596, 1600,  572, 520,  570, 1604,  572, 1598,  572, 520,  570, 520,  570, 1600,  570, 522,  570, 522,  570, 1606,  570, 520,  570, 520,  572, 1600,  570, 1600,  572, 518,  570, 1608,  568, 522,  570, 520,  570, 1604,  570, 1600,  570, 1600,  570, 1600,  570, 1600,  570, 1602,  572, 1604,  570, 1600,  572, 520,  570, 520,  570, 520,  570, 518,  572, 520,  570, 524,  568, 522,  568, 1602,  570, 520,  568, 1606,  570, 520,  570, 520,  570, 520,  570, 522,  570, 1608,  570, 520,  570, 1604,  570, 522,  570, 1604,  570, 1602,  570, 1602,  570, 1604,  636};
uint16_t DESLIGA[199] = {4440, 4310,  598, 1600,  570, 520,  570, 1606,  568, 1600,  572, 520,  570, 520,  570, 1600,  570, 522,  570, 522,  570, 1606,  568, 520,  572, 520,  570, 1600,  572, 1600,  570, 520,  570, 1606,  570, 522,  570, 1600,  570, 1602,  570, 1602,  570, 1600,  570, 520,  570, 1604,  570, 1602,  570, 1608,  570, 518,  572, 518,  570, 520,  570, 520,  572, 1600,  570, 520,  570, 524,  568, 1610,  570, 1600,  572, 1600,  570, 520,  572, 520,  570, 520,  570, 520,  570, 522,  572, 522,  570, 520,  572, 520,  570, 1606,  570, 1600,  572, 1600,  570, 1602,  570, 1602,  634, 5098,  4458, 4310,  596, 1602,  572, 518,  570, 1604,  572, 1600,  570, 520,  570, 520,  572, 1600,  570, 520,  572, 520,  570, 1604,  572, 520,  570, 520,  570, 1600,  572, 1600,  594, 496,  572, 1606,  570, 522,  572, 1600,  570, 1600,  570, 1602,  570, 1600,  570, 520,  572, 1604,  596, 1578,  572, 1604,  572, 518,  572, 518,  572, 518,  570, 520,  572, 1600,  570, 520,  572, 520,  572, 1606,  594, 1578,  570, 1598,  572, 520,  570, 520,  572, 518,  572, 520,  570, 522,  570, 522,  570, 520,  570, 520,  570, 1606,  572, 1598,  572, 1600,  572, 1600,  570, 1604,  634};

//DHT
#include "DHT.h"
#define DHTTYPE DHT11   // DHT 11

// CONEXAO WIFI
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

String ultimo_comando = "NENHUM";
 
String page = "";
int LED2 = D2;
int LED3 = D3;
int LED4 = D4;

void setup(void){
  dht.begin();
  //HTML PARA A PÁGINA A EXIBIR
  //page = "<h1>ACIONAMENTO CONDICIONADOR DE AR WIFI - TMECA 28</h1><p>COMANDO - TEMP: 22 / MODO: COOL / FAN: MAX &nbsp;&nbsp;&nbsp;<a href=\"LED2On\"><button>ON</button></a>&nbsp;<a href=\"LED2Off\"><button>OFF</button></a></p><h2>LEITURAS:<h2><table><tr><td><b>ULTIMO COMANDO RECEBIDO:</b></td><td>[ON]:</td></tr><tr><td><b>TEMPERATURA:</b></td><td>[12.34]:</td></tr><tr><td><b>UMIDADE:</b></td><td>[45.67]</td></tr></table>";
  //make the LED pin output and initially turned off
  leituradht();
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
  server.on("/DISPOn", [](){
    ultimo_comando = "ON";
    leituradht();
    server.send(200, "text/html", page);
    irsend.sendRaw(LIGA, 199, 38);  // Send a raw data capture at 38kHz.
    delay(1000);
  });
  server.on("/DISPOff", [](){
    ultimo_comando = "OFF";
    leituradht();
    server.send(200, "text/html", page);
    irsend.sendRaw(DESLIGA, 199, 38);  // Send a raw data capture at 38kHz.
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
            

            //COLOCA OS DADOS NA HTML
            
              page = "<h1>ACIONAMENTO CONDICIONADOR DE AR WIFI - TMECA 28</h1><p>COMANDO - TEMP: 22 / MODO: COOL / FAN: MAX &nbsp;&nbsp;&nbsp;<a href=\"DISPOn\"><button>ON</button></a>&nbsp;<a href=\"DISPOff\"><button>OFF</button></a></p><h2>LEITURAS:<h2><table><tr><td><b>ULTIMO COMANDO RECEBIDO:</b></td><td>[";
              page +=(ultimo_comando);
              page +="]:</td></tr><tr><td><b>TEMPERATURA:</b></td><td>[";
              page +=(t);
              page +="]:</td></tr><tr><td><b>UMIDADE:</b></td><td>[";
              page +=(h);
              page +="]</td></tr></table>";

            }
            //FIM DA VOID leituradht
}
