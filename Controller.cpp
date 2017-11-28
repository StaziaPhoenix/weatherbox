#include "Arduino.h"

#define pump 6
#define mister 3
#define fan 5

class Controller {
  private:


  public:

    Controller () {}
  
    void pumpOn() {
      analogWrite(pump, 200);
    }
    
    void pumpOff() {
      analogWrite(pump, 0);
    }
    
    void misterOn() {
      analogWrite(mister, 255);
    }
    
    void misterOff() {
      analogWrite(mister, 0);
    }
    
    void fanOn() {
      analogWrite(fan, 255);
    }
    
    void fanOff() {
       analogWrite(fan, 0);
    }

    void allOn() {
      pumpOn();
      misterOn();
      fanOn();
    }

    void allOff() {
      pumpOff();
      misterOff();
      fanOff();
    }

    void clearDisplay(int seconds) {
      allOff();
      fanOn();
      delay(seconds*1000);
      fanOff();
    }
};

