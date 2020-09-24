#ifndef EEPROMData_h
#define EEPROMData_h

#include <Arduino.h>

class EEPROMData{
private:
  const int start_index = 0;
  const int data_length = 100;
  String readData(int start, int length);
  void writeData(String data, int start, int length);
public:
  void begin();
  String readWifiSSID();
  String readWifiPassword();
  String readMQTTBroker();
  String readDeviceNumber();
  String readDeviceTotal();
  String readUsername();
  String readPassword();
  void writeWifiSSID(String data);
  void writeWifiPassword(String data);
  void writeMQTTBroker(String data);
  void writeDeviceNumber(String data);
  void writeDeviceTotal(String data);
  void writeUsername(String data);
  void writePassword(String data);
};

#endif
