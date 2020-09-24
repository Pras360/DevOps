#ifndef IRDistance_h
#define IRDistance_h

#include <Arduino.h>

class IRDistance{
private:
  int pin;

public:
  IRDistance(int pin);
  int read();
};

#endif
