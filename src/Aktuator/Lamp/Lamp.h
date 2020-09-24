#ifndef Lamp_h
#define Lamp_h

#include <Arduino.h>

class Lamp{
private:
  int pin;
  void setDutyCycle(int val);
public:
  Lamp(int pin);
  void setDim();
  void setBright();
};

#endif
