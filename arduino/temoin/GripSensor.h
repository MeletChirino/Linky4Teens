#ifndef GripSensor_h
#define GripSensor_h
#include "RGBLed.h"

class GripSensor {
  private:
    int _low_position_pin, _high_position_pin;
    bool _debug, _good_relay;
    bool _low_position_grip, _high_position_grip,
         _low_position_change, _high_position_change;
    void _read_grip();
  public:
    GripSensor(int, int);
    void init();
    void read_grip();
    void print_status(bool);
    bool grip_ok();
    bool high_grip();
    void print_status();
    bool low_grip();
    void enable_debug();
    bool good_relay();
    void reset_relay(RGBLed);
};

#endif
