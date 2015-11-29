#include <Ultrasonic.h>
#include <rgb_lcd.h>
#include <LWiFi.h>
#include <LWiFiClient.h>
#include <LWiFiServer.h>
#include <LWiFiUdp.h>
#include <rgb_lcd.h>

#include "IotHttpClient.h"
#include "IotUtils.h"

#include <Ultrasonic.h>
Ultrasonic ultrasonic(4); //**the number is the port number

#include <Wire.h>
#include "rgb_lcd.h"

#define WIFI_SSID                       "Indras"
#define WIFI_PASSWORD                   "12345678"

rgb_lcd lcd;
const int colorR = 0;
const int colorG = 255; //by default seat available so colour is green
const int colorB = 0;

void setup()
{
  Serial.begin(9600);
  lcd.begin(16, 2);
  lcd.setRGB(colorR, colorG, colorB);
  LWiFi.begin();
  
  //see if connection to cloud is okay
  Serial.print("\nSearching for Wifi AP...\n");

    if ( LWiFi.connectWPA(WIFI_SSID, WIFI_PASSWORD) != 1 )
    {
        Serial.println("Failed to Connect to Wifi.");
    }
    else
    {
        Serial.println("Connected to WiFi");
    }
  delay(6000);
}
void loop()
{
  
 
  ultrasonic.MeasureInCentimeters();
  
  int range = ultrasonic.RangeInCentimeters;
  srand(ultrasonic.RangeInCentimeters);
  
  //construct JSON data string using the data
  std::string json_iot_data;
        
  json_iot_data += "{ \"Range\":" + IotUtils::IntToString(range);
  json_iot_data += " }";
  
  // Send the JSON data to the Azure cloud and get the HTTP status code.
  IotHttpClient     https_client;
        
  int http_code = https_client.SendAzureHttpsData(json_iot_data);

  Serial.println("Print HTTP Code:" + String(http_code));
        //lcd.setCursor(0, 1);
        //lcd.print("Code:" + String(http_code));
  
  Serial.print("Distance: ");
  Serial.print(ultrasonic.RangeInCentimeters);
  Serial.println(" cm");
  
  if(range < 30 ){
  
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Occupied!");
  lcd.setRGB(255, 0, 0);
  
  delay(1000);
  }
  else {
  
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Available");
  lcd.setRGB(0, 255, 0); 
  
  delay(10000);
  
  }
}
