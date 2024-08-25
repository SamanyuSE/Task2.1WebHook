#include <WiFiNINA.h>
#include "secrets.h"
#include "DHT.h"
#include "ThingSpeak.h" 

#define DHTPIN 2     

#define DHTTYPE DHT22   

char ssid[] = OnePlus;   // your network SSID (name) 
char pass[] = pt123456789;   // your network password
int keyIndex = 0;            
WiFiClient  client;

unsigned long myChannelNumber = SECRET_CH_ID;
const char * myWriteAPIKey = SECRET_WRITE_APIKEY;

DHT dht(DHTPIN, DHTTYPE);

String myStatus = "";

void setup() {
  Serial.begin(115200);      
  while (!Serial) {
    ; 
  }
  dht.begin();
  
  ThingSpeak.begin(client);  
}

void loop() {


  if(WiFi.status() != WL_CONNECTED){
    Serial.print("Attempting to connect to SSID: ");
    Serial.println(SECRET_SSID);
    while(WiFi.status() != WL_CONNECTED){
      WiFi.begin(ssid, pass);  
      Serial.print(".");
      delay(5000);     
    } 
    Serial.println("\nConnected.");
  }
  float humidity = dht.readHumidity();
  float temp = dht.readTemperature();

 
  if (isnan(humidity) || isnan(temp) ) {
    Serial.println(F("Failed to read from DHT sensor!"));
    return;
  }

  
  ThingSpeak.setField(1, humidity);
  ThingSpeak.setField(2, temp);

  
  myStatus = "Updated";
  ThingSpeak.setStatus(myStatus);
  
  int x = ThingSpeak.writeFields(myChannelNumber, myWriteAPIKey);
  if(x == 200){
    Serial.println("Channel update successful.");
  }
  else{
    Serial.println("Problem updating channel. HTTP error code " + String(x));
  }
  int z = 0;
  Serial.println(z);
  delay(60000); 
  z++;
}