#ifndef Monitoring_h
#define Monitoring_h

#include <Arduino.h>
#include <ESP8266HTTPClient.h>

class Monitoring{
private:
    String _server;
    String _key;
    int _deviceNumber;

public:
    void begin(String server, String key, int deviceNumber);
    String exec(bool statusLamp);
};

#endif