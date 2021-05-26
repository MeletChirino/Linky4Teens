#ifndef GripSensor_h
#define GripSensor_h
#include "RGBLed.h"
/** 
 * \class GripSensor
 * \brief Classe qui gere les capteur touch.
 * \details Cette classe gere les deux capteurs touch
 * qui sont utilisées pour savoir si l'athlete a bien pris 
 * le temoin par un des deux cotés.
 */
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
    bool grip_ok();
    bool high_grip();
    void print_status();
    bool low_grip();
    bool good_relay();
};

#endif
