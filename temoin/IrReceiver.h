#ifndef IrReceiver_h
#define IrReceiver_h

#include "Arduino.h"

class IrReceiver{
  private:
    int _pin;
  public:
    IrReceiver(int);
    void init();
    bool get_status();
  };


#endif
