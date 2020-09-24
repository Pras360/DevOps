#include "Lamp.h"
#include <Arduino.h>

//Private
void Lamp::setDutyCycle(int val){
  /*
  Spesifikasi :
  - Prosedur ini digunakan untuk mengatur duty cycle sinyal PWM pada keluaran pin Led.
  */
  int _8bit_pwm = (val * 255) / 100;
  analogWrite(this->pin, _8bit_pwm);
}
//------------------------------------------------------------------------------

//Public
Lamp::Lamp(int pin){
  /*
  Spesifikasi :
  - Prosedur ini digunakan untuk inisialisasi pin Led dengan keluaran berupa sinyal PWM.
  */
  this->pin = pin;
  pinMode(this->pin, OUTPUT);
}

void Lamp::setDim(){
  setDutyCycle(5);
}

void Lamp::setBright(){
  setDutyCycle(100);
}
//------------------------------------------------------------------------------