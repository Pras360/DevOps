#include "OTAUpdate.h"
#include <Arduino.h>

void OTAUpdate::begin(String server){
    this->_server = server;
}

void OTAUpdate::loop(){
    if(WiFi.status() == WL_CONNECTED) {
        Serial.println("Update");
        HTTPClient http;
        String str_get = "";
        str_get += this->_server;
        str_get += "/data?get";
        http.begin(str_get);
        Serial.println(str_get);
        int httpCode = http.GET();
        if(httpCode == HTTP_CODE_OK){
            DynamicJsonDocument doc(this->_capacity);
            deserializeJson(doc, http.getString());
            JsonObject obj = doc.as<JsonObject>();
            String release = obj["release"].as<String>();
            String type = obj["type"].as<String>();
            String update_type = obj["update_type"].as<String>();
            String update_status = obj["update_status"].as<String>();
            //Serial.println(release);
            //Serial.println(type);
            //Serial.println(update_type);
            //Serial.println(update_status);
            if(update_status == "enable"){
                Serial.println("Update Enable");
                if(type == update_type){
                    Serial.println("Jenis tag release sama");
                    if(release != buildTag){
                        Serial.println("Firmware baru ditemukan, melakukan update");
                        String server_update;
                        server_update = this->_server;
                        server_update += "/firmware";
                        Serial.println(server_update);
                        t_httpUpdate_return ret = ESPhttpUpdate.update(server_update);
                        
                        switch(ret) {
                            case HTTP_UPDATE_FAILED:
                            Serial.printf("UPDATE ERROR (%d): %s", ESPhttpUpdate.getLastError(), ESPhttpUpdate.getLastErrorString().c_str());
                            Serial.printf("\nUPDATE ERROR (%d): %s\n", ESPhttpUpdate.getLastError(), ESPhttpUpdate.getLastErrorString().c_str());
                            break;

                            case HTTP_UPDATE_NO_UPDATES:
                            Serial.println(" Already in Current Version");
                            Serial.println("Already in Current Version");
                            Serial.println(buildTag);
                            Serial.flush();
                            break;
                                    
                            case HTTP_UPDATE_OK:
                            Serial.println("Updating FIrmware...");
                            Serial.println("Updating FIrmware...");
                            Serial.flush();
                            break;
                        }
                    }else{
                        Serial.println("Tidak ada update terbaru");
                    }
                }
            }else{
                Serial.println("Update Disable");
            }
        }
    }
}