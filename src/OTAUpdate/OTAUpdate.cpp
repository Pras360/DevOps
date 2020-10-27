#include "OTAUpdate.h"
#include <Arduino.h>

void OTAUpdate::begin(String server){
    this->_server = server;
}

void OTAUpdate::loop(){
    if(WiFi.status() == WL_CONNECTED) {
        Serial.println("Update");
        String server_update;
        server_update = this->_server;
        server_update += "?tag=";
        server_update += buildTag;
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
    }
}