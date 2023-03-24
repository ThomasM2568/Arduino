#include <Arduino.h>

class UltraSonic {
  private:
  const int echo= A4;
  const int trig = A5;

  public:
  UltraSonic(){
    pinMode(echo,INPUT);
    pinMode(trig,OUTPUT);
  }

  float getDistance(){
    digitalWrite(trig,LOW);
    delayMicroseconds(2);
    digitalWrite(trig, HIGH);
    delayMicroseconds(20);
    digitalWrite(trig,LOW);
    float d=pulseIn(echo,HIGH);
    d/=5.8f;
    return d;
  }
};
