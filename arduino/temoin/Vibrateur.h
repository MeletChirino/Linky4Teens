#ifndef Vibrateur_h
#define Vibrateur_h
/** 
 * \class Vibrateur
 * \brief Classe qui gere un moteur vibrateur.
 */
class Vibrateur {
  private:
    int _pin;
  public:
    Vibrateur(int);
    void init();
    void stop();
    /*
       next functions are for testing pourposes,
       down below there are nicer functions with
       set delays
    */
    void vibrate(int);
    void vibrate_time(int, int);
};

#endif
