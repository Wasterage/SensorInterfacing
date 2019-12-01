#include <ESP8266HTTPClient.h>
#include <ESP8266WiFi.h>
#include <ArduinoJson.h>

const int pingPin = 2; // Trigger Pin of Ultrasonic Sensor
const int echoPin = 0; // Echo Pin of Ultrasonic Sensor
const String lat = ""; //bin lat
const String lng = ""; //bin lng
void setup() {
 
  Serial.begin(115200);                            //Serial connection
  WiFi.begin("SSID", "PASS");   //WiFi connection
 
  while (WiFi.status() != WL_CONNECTED) {  //Wait for the WiFI connection completion
 
    delay(500);
    Serial.println("Waiting for connection");
 
  }
 
}
 
void loop() {
 
  if (WiFi.status() == WL_CONNECTED) { //Check WiFi connection status
    long duration, cm;
   pinMode(pingPin, OUTPUT);
   digitalWrite(pingPin, LOW);
   delayMicroseconds(2);
   digitalWrite(pingPin, HIGH);
   delayMicroseconds(10);
   digitalWrite(pingPin, LOW);
   pinMode(echoPin, INPUT);
   duration = pulseIn(echoPin, HIGH);
   cm = microsecondsToCentimeters(duration);
   Serial.print(cm);
   Serial.println();
   if(cm<10 && cm >0){
        StaticJsonBuffer<300> JSONbuffer;   //Declaring static JSON buffer
    JsonObject& JSONencoder = JSONbuffer.createObject(); 
 
    JSONencoder["lat"] = lat;
    JSONencoder["lng"] = lng;
 
    char JSONmessageBuffer[300];
    JSONencoder.prettyPrintTo(JSONmessageBuffer, sizeof(JSONmessageBuffer));
    Serial.println(JSONmessageBuffer);
 
    HTTPClient http;    //Declare object of class HTTPClient
 
    http.begin("");      //Specify request destination
    http.addHeader("Content-Type", "application/json");  //Specify content-type header
 
    int httpCode = http.POST(JSONmessageBuffer);   //Send the request
    String payload = http.getString();                                        //Get the response payload
 
    //Serial.println(httpCode);   //Print HTTP return code
    Serial.println(payload);    //Print request response payload
 
    http.end(); 
    } else {
      Serial.print("Not full yet");
      }
   //Close connection
 
  } else {
 
    Serial.println("Error in WiFi connection");
 
  }
 
  delay(3000);  //Send a request every 30 seconds
 
}
long microsecondsToCentimeters(long microseconds) {
   return microseconds / 29 / 2;
}
