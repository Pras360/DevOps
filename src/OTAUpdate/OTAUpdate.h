#ifndef OTAUpdate_h
#define OTAUpdate_h

#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <ESP8266httpUpdate.h>

#define TEXTIFY(A) #A
#define ESCAPEQUOTE(A) TEXTIFY(A)

class OTAUpdate{
private:
    String _server;
    String buildTag = ESCAPEQUOTE(BUILD_TAG);
public:
    void begin(String server);
    void loop();
};

#endif