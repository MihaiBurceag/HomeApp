/*
  Simple Wifi Switch with DHT-11 sending data to DOMOTICZ
  */

#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>
#include <DHT.h>

MDNSResponder mdns;

// Replace with your network credentials
const char* ssid = "XXXX";
const char* password = "XXXXX";

ESP8266WebServer server(80);
WiFiClient client;
String web_on_html = "<h1>SONOFF switch is ON</h1><p><a href=\"on\"><button>ON</button></a>&nbsp;<a href=\"off\"><button>OFF</button></a></p>";
String web_off_html = "<h1>SONOFF switch is OFF</h1><p><a href=\"on\"><button>ON</button></a>&nbsp;<a href=\"off\"><button>OFF</button></a></p>";

int gpio_13_led = 13;
int gpio_12_relay = 12;

#define DHTTYPE DHT11 // DHT11 or DHT22
#define DHTPIN  14
#define dhtInterval 2000 //(ms) - 2 seconds between reading
const float avgForce = 10.0;  // Force of which the averaging function is using to affect the reading
const float avgSkip = 0.5; // Skip the averaging function if the reading difference is bigger
float avgHumidity, avgTemperature; 
unsigned long dhtLast = 0;
DHT dht(DHTPIN, DHTTYPE, 11);

// domoticz
const char * domoticz_server = "192.168.100.17"; //Domoticz port
int port = 8080; //Domoticz port
int idx = 9; //IDX for this virtual sensor, found in Setup -> Devices
#define dhtPrint 60000 //(ms) - 1 minute wait before printing data
unsigned long dhtPrintLast = 0;

void printInfo()
 {
    // Domoticz format /json.htm?type=command&param=udevice&idx=IDX&nvalue=0&svalue=TEMP;HUM;HUM_STAT

    if (client.connect(domoticz_server,port)) {

        client.print("GET /json.htm?type=command&param=udevice&idx=");
        client.print(idx);
        client.print("&nvalue=0&svalue=");
        client.print(avgTemperature);
        client.print(";");
        client.print(avgHumidity);
        client.print(";0"); //Value for HUM_STAT. Can be one of: 0=Normal, 1=Comfortable, 2=Dry, 3=Wet
        client.println(" HTTP/1.1");
        client.print("Host: ");
        client.print(domoticz_server);
        client.print(":");
        client.println(port);
        client.println("User-Agent: Arduino-ethernet");
        client.println("Connection: close");
        client.println();
        
        client.stop();
     }
  }
void setup(void){  
  //  Init
  pinMode(gpio_13_led, OUTPUT);
  digitalWrite(gpio_13_led, HIGH);
  
  pinMode(gpio_12_relay, OUTPUT);
  digitalWrite(gpio_12_relay, HIGH);
 
  delay(5000);

  WiFi.begin(ssid, password);

  // Wait for connection
  while (WiFi.status() != WL_CONNECTED) {
    digitalWrite(gpio_13_led, LOW);
    delay(500);
    digitalWrite(gpio_13_led, HIGH);
    delay(500);
  }
  

  digitalWrite(gpio_13_led, HIGH);
  digitalWrite(gpio_12_relay, LOW);  
  
  server.on("/", [](){
    if(digitalRead(gpio_12_relay)==HIGH) {
      server.send(200, "text/html", web_on_html);
    } else {
      server.send(200, "text/html", web_off_html);
    }
  });
  
  server.on("/on", [](){
    server.send(200, "text/html", web_on_html);
    digitalWrite(gpio_13_led, LOW);
    digitalWrite(gpio_12_relay, HIGH);
    delay(1000);
  });
  
  server.on("/off", [](){
    server.send(200, "text/html", web_off_html);
    digitalWrite(gpio_13_led, HIGH);
    digitalWrite(gpio_12_relay, LOW);
    delay(1000); 
  });
  
  server.begin();

}
void readDHT()
{
  if (millis() - dhtLast < dhtInterval) return;
  dhtLast = millis();
 
  float tempHumi = dht.readHumidity();
  float tempTemp = dht.readTemperature();
 
  if (isnan(tempHumi) || isnan(tempTemp))
  {
    Serial.println("Failed to read from DHT sensor!");
    return;
  }
 
  if (abs(tempTemp - avgTemperature) > avgSkip)
  {
    avgTemperature = tempTemp;
  }
  else
  {
    avgTemperature = (avgTemperature *  (100.0-avgForce)/100.0) + ((avgForce/100.0) * tempTemp);
  }
 
  if (abs(tempHumi - avgHumidity) > avgSkip)
  {
    avgHumidity = tempHumi;
  }
  else
  {
    avgHumidity = (avgHumidity * ((100.0-avgForce)/100.0)) + ((avgForce/100.0) * tempHumi);
  }
  if (millis() - dhtPrintLast < dhtPrint) return;
  dhtPrintLast = millis();
  printInfo();
} 

void loop(void){
  server.handleClient();
  readDHT();
} 
