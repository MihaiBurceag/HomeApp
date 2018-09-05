#include <SparkFunHTU21D.h>
int ledPin = 13;             
int inputPin = 2;        
int pirState = LOW;          
int val = 0;  
long highTime = 0;
long tempReadInterval=10000;
long lastTime=0;
int incomingByte = 0; 
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
    //Serial.begin(9600);
    Serial.println("{\"temp\":\"" + String(temp,4) + "\",\"hum\":\"" + String(humd,4) + "\"}");
    //Serial.end();
  }
  
  
}
void loop(){
  val = digitalRead(inputPin);  
  if (Serial.available() > 0) {
    // read the incoming byte:
    incomingByte = Serial.read();
    // say what you got:
    Serial.print("I received: ");
    Serial.println(incomingByte, DEC);
        }
  temp(); 
  if (val == HIGH) {            
    digitalWrite(ledPin, HIGH); 
    if (pirState == LOW) {
      // we have just turned on
      //Serial.begin(9600);
      Serial.println("{\"pir\":\"true\"}");
      //Serial.end();
     // Serial.println();
      pirState = HIGH;
    }
  } else {
    digitalWrite(ledPin, LOW); 
    if (pirState == HIGH){
      // we have just turned of
      //Serial.begin(9600);
      Serial.println("{\"pir\":\"false\"}");
      //Serial.end();
     // Serial.println();
      pirState = LOW;
    }
  }
 // delay(500);
}
