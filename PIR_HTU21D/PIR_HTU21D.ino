#include <SparkFunHTU21D.h>



/*
 * PIR sensor tester
 */
 
int ledPin = 12;                // choose the pin for the LED
int inputPin = 2;               // choose the input pin (for PIR sensor)
int pirState = LOW;             // we start, assuming no motion detected
int val = 0;   // variable for reading the pin status
long highTime = 0;
long tempReadInterval=60000;
long lastTime=0;
HTU21D myHumidity; 
void setup() {
  pinMode(ledPin, OUTPUT);     
  pinMode(inputPin, INPUT);    
 
  Serial.begin(9600);
  myHumidity.begin();
}

void temp(){
  long now = millis();
  if ( now - lastTime > tempReadInterval) {
    lastTime=now;
    float humd = myHumidity.readHumidity();
    float temp = myHumidity.readTemperature();
    String jStr = "{\"temp\":\"";
    jStr += temp;
    jStr += "\",\"hum\":\"";
    jStr += humd;
    jStr += "\"}";
    Serial.println(jStr);
    Serial.println();
  }
  
  
}
void loop(){
  val = digitalRead(inputPin);  
  temp(); 
  if (val == HIGH) {            
    digitalWrite(ledPin, HIGH); 
    if (pirState == LOW) {
      // we have just turned on
      Serial.println("{\"pir\":\"true\"}");
      Serial.println();
      pirState = HIGH;
    }
  } else {
    digitalWrite(ledPin, LOW); 
    if (pirState == HIGH){
      // we have just turned of
      Serial.println("{\"pir\":\"false\"}");
      Serial.println();
      pirState = LOW;
    }
  }
  
}
