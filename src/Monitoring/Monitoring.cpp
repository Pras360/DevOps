#include "Monitoring.h"
#include <Arduino.h>

void Monitoring::begin(String server, String key, int deviceNumber){
    this->_server = server;
    this->_key = key;
    this->_deviceNumber = deviceNumber;
}

String Monitoring::exec(bool statusLamp){
    HTTPClient http;
      String str_get = "";
      str_get += this->_server;
      str_get += "?access_key=";
      str_get += this->_key;
      str_get += "&d";
      str_get += this->_deviceNumber;
      str_get += "=";
      str_get += statusLamp;
      http.begin(str_get);
      int httpCode = http.GET();
      if(httpCode == HTTP_CODE_OK){
          return http.getString();
      }else{
          return "";
      }
}