/*
  Rui Santos
  Complete project details at https://RandomNerdTutorials.com
  
  Permission is hereby granted, free of charge, to any person obtaining a copy
  of this software and associated documentation files.
  
  The above copyright notice and this permission notice shall be included in all
  copies or substantial portions of the Software.

*/

//#ifdef ESP32
//  #include <WiFi.h>
 // #include <HTTPClient.h>
//#else
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <WiFiClient.h>
//#endif

//#include <Wire.h>
//#include <Adafruit_Sensor.h>
//#include <Adafruit_BME280.h>
#include "DHT.h"
#define ON_Board_LED 2
#define DHTPIN D2
#define DHTTYPE DHT22 
DHT dht(DHTPIN, DHTTYPE);

// Replace with your network credentials
const char* ssid     = "_________";
const char* password = "______________";

// REPLACE with your Domain name and URL path or IP address with path
const char* serverName = "http://192.168.100.243/esp8266/post-data.php";

// Keep this API Key value to be compatible with the PHP code provided in the project page. 
// If you change the apiKeyValue value, the PHP file /post-data.php also needs to have the same key 
String apiKeyValue = "tPmAT5Ab3j7F9";

/*#include <SPI.h>
#define BME_SCK 18
#define BME_MISO 19
#define BME_MOSI 23
#define BME_CS 5*/



void setup() {
  Serial.begin(115200);
  delay(500);

  dht.begin();
  
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  Serial.println("Connecting");

  pinMode(ON_Board_LED,OUTPUT); //--> On Board LED port Direction output
  digitalWrite(ON_Board_LED, HIGH); //--> Turn off Led On Board

  
  while(WiFi.status() != WL_CONNECTED) { 
    delay(500);
    Serial.print(".");
    digitalWrite(ON_Board_LED, LOW);
    delay(250);
    digitalWrite(ON_Board_LED, HIGH);
    delay(250);
  }
  digitalWrite(ON_Board_LED, HIGH);
  Serial.println("");
  Serial.print("Connected to WiFi network with IP Address: ");
  Serial.println(WiFi.localIP());

  // (you can also pass in a Wire library object like &Wire2)
  //bool status = bme.begin(0x76);
  //if (!status) {
    //Serial.println("Could not find a valid BME280 sensor, check wiring or change I2C address!");
    //while (1);
  //}
}

void loop() {
  float g = 10;
  float h = dht.readHumidity();
  float t = dht.readTemperature();
  if(isnan(h) || isnan(t)){
    Serial.println("Failed to read from DHT sensor!");
    return;
    }
  //Check WiFi connection status
  //if(WiFi.status()== WL_CONNECTED){
    HTTPClient http;
    
    // Your Domain name with URL path or IP address with path
    http.begin(serverName);
    
    // Specify content-type header
    http.addHeader("Content-Type", "application/x-www-form-urlencoded");
    
    // Prepare your HTTP POST request data
    String httpRequestData = "api_key=" + apiKeyValue + "&value1=" + String(t)
                           + "&value2=" + String(h) + "&value3=" + String(g) + "";
    Serial.print("httpRequestData: ");
    Serial.println(httpRequestData);
    
    // You can comment the httpRequestData variable above
    // then, use the httpRequestData variable below (for testing purposes without the BME280 sensor)
    //String httpRequestData = "api_key=tPmAT5Ab3j7F9&value1=24.75&value2=49.54&value3=1005.14";

    // Send HTTP POST request
    int httpResponseCode = http.POST(httpRequestData);
     
    // If you need an HTTP request with a content type: text/plain
    //http.addHeader("Content-Type", "text/plain");
    //int httpResponseCode = http.POST("Hello, World!");
    
    // If you need an HTTP request with a content type: application/json, use the following:
    //http.addHeader("Content-Type", "application/json");
    //int httpResponseCode = http.POST("{\"value1\":\"19\",\"value2\":\"67\",\"value3\":\"78\"}");
    
    if (httpResponseCode>0) {
      Serial.print("HTTP Response code: ");
      Serial.println(httpResponseCode);
    }
    else {
      Serial.print("Error code: ");
      Serial.println(httpResponseCode);
    }
    // Free resources
    http.end();
  //}
  //else {
    //Serial.println("WiFi Disconnected");
  //}
  //Send an HTTP POST request every 30 seconds
  delay(30000);  
}
