#include <ESP8266WiFi.h>          //https://github.com/esp8266/Arduino
#include <DNSServer.h>
#include <ESP8266WebServer.h>
#include <WiFiManager.h>         //https://github.com/tzapu/WiFiManager
#include <ESP8266HTTPClient.h>
#include <ESP8266httpUpdate.h>
#include <Ticker.h>
//================Version Steble or Unstable Conf ========
bool stable = true;

//==============================INISIALISASI=====================================
#define TEXTIFY(A) #A
#define ESCAPEQUOTE(A) TEXTIFY(A)
String buildTag = ESCAPEQUOTE(BUILD_TAG);

int clear = 0;
const int ESP_LED = 2;
int ledPin =  4; //D2     // the number of the LED pin
int button = 14; //D1
void enableUpdateCheck();
Ticker updateCheck(enableUpdateCheck, 30000); // timer for check update with interval 30s
bool doUpdateCheck = true;
bool download = true;
WiFiManager wifiManager;

//=================== PROSEDUR & FUNGSI =====================
void enableUpdateCheck() {
  doUpdateCheck = true;
}

//==================Wifi-Setting==========================
void resetWifi(){
  wifiManager.resetSettings();
  delay(1000);
  ESP.reset();
  delay(3000);
}


//=====================================Download Firmware=====================================
void DownloadBin(){

  Serial.println("Checking Firmware...");  
  if (WiFi.status() == WL_CONNECTED) {
      //==========================downloading firmware.bin with HTTP OTA================
        t_httpUpdate_return ret = ESPhttpUpdate.update("http://pras-devops.firmandev.com/index.php?tag="+ buildTag );
        switch(ret) {
         case HTTP_UPDATE_FAILED:
          Serial.printf("UPDATE ERROR (%d): %s", ESPhttpUpdate.getLastError(), ESPhttpUpdate.getLastErrorString().c_str());
          break;

         case HTTP_UPDATE_NO_UPDATES:
          Serial.println(" You're in Current Version");

          break;
          
         case HTTP_UPDATE_OK:
          Serial.println("Updating FIrmware...");
          delay(1000);
       }
    }     
}

void setup(){

  wifiManager.autoConnect("DevOps");
  updateCheck.start();
  pinMode(ledPin, OUTPUT);
  pinMode(button, INPUT_PULLUP);
  pinMode(ESP_LED, OUTPUT);
}

int ledState = 0;             // ledState used to set the LED
unsigned long previousMillis = 0;        // will store last time LED was updated
unsigned long currentMillis;
long OnTime = 250;           // milliseconds of on-time
long OffTime = 750;

int temp = 1;
int hold = 0;
int counter = 0;

void loop(){
  digitalWrite(ledPin, HIGH);
  delay(1000);
  digitalWrite(ledPin, LOW);
  delay(1000);
  updateCheck.update();

  //========Bagian Program Utama, sesuaikan alatmu=========
  temp = digitalRead(button);
  if ( temp != hold){
    if ( temp == LOW ){
      counter++;
      Serial.println(counter);// asd
    }
    delay(50); //delay untuk menghindari bouncing
  }  
  hold = temp;
 if (counter == 2){
   currentMillis = millis();
 
 if((ledState == HIGH) && (currentMillis - previousMillis >= OnTime))
  {
    ledState = LOW;  // Turn it off
    previousMillis = currentMillis;  // Remember the time
    digitalWrite(ledPin, ledState);  // Update the actual LED
  }
  else if ((ledState == LOW) && (currentMillis - previousMillis >= OffTime))
  {
    ledState = HIGH;  // turn it on
    previousMillis = currentMillis;   // Remember the time aaa
    digitalWrite(ledPin, ledState);   // Update the actual LED
  }
 }
 if (counter ==3 ){
   counter = 0;
   currentMillis = 0;
   digitalWrite(ledPin, LOW);
 } 
 
  //======== Batas akhir program utama ===================

  if (doUpdateCheck == true){
    DownloadBin();
    doUpdateCheck = false;
  }
}
