#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
 
// Replace with your network credentials
const char* ssid = "ACEM28";
const char* password = "japoneisdoidao";
 
ESP8266WebServer server(80);   //instantiate server at port 80 (http port)
 
String page = "";
int LED2 = D2;
int LED3 = D3;
int LED4 = D4;

void setup(void){
  //the HTML of the web page
  page = "<h1>ACIONAMENTO WIFI - TMECA 28</h1><p>DISPOSITIVO A &nbsp;&nbsp;&nbsp;<a href=\"LED2On\"><button>ON</button></a>&nbsp;<a href=\"LED2Off\"><button>OFF</button></a></p><p>DISPOSITIVO B &nbsp;&nbsp;&nbsp;<a href=\"LED3On\"><button>ON</button></a>&nbsp;<a href=\"LED3Off\"><button>OFF</button></a></p><p>DISPOSITIVO C &nbsp;&nbsp;&nbsp;<a href=\"LED4On\"><button>ON</button></a>&nbsp;<a href=\"LED4Off\"><button>OFF</button></a></p>";
  //make the LED pin output and initially turned off
  pinMode(LED2, OUTPUT);
  pinMode(LED3, OUTPUT);
  pinMode(LED4, OUTPUT);
  
  digitalWrite(LED2, LOW);
  digitalWrite(LED3, LOW);
  digitalWrite(LED4, LOW);
  
   
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
  });

  //-----------------------------------------------
  server.on("/LED2On", [](){
    server.send(200, "text/html", page);
    digitalWrite(LED2, HIGH);
    delay(1000);
  });
  server.on("/LED2Off", [](){
    server.send(200, "text/html", page);
    digitalWrite(LED2, LOW);
    delay(1000); 
  });
  //-----------------------------------------------

  //-----------------------------------------------
  server.on("/LED3On", [](){
    server.send(200, "text/html", page);
    digitalWrite(LED3, HIGH);
    delay(1000);
  });
  server.on("/LED3Off", [](){
    server.send(200, "text/html", page);
    digitalWrite(LED3, LOW);
    delay(1000); 
  });
  //-----------------------------------------------

  //-----------------------------------------------
  server.on("/LED4On", [](){
    server.send(200, "text/html", page);
    digitalWrite(LED4, HIGH);
    delay(1000);
  });
  server.on("/LED4Off", [](){
    server.send(200, "text/html", page);
    digitalWrite(LED4, LOW);
    delay(1000); 
  });
  //-----------------------------------------------
  
  server.begin();
  Serial.println("Web server started!");
}
 
void loop(void){
  server.handleClient();
}
