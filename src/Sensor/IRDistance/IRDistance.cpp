#include "IRDistance.h"
#include <Arduino.h>

//Public
IRDistance::IRDistance(int pin){
  /*
  Spesifikasi :
  - Constructor ini digunakan untuk konfigurasi pin sensor.
  */

  this->pin = pin;
  pinMode(this->pin, INPUT);
}

int IRDistance::read(){
  /*
  Spesifikasi : 
  - fungsi ini digunakan untuk membaca nilai sensor.
  */
 
  return analogRead(this->pin);  
}
//------------------------------------------------------------------------------
