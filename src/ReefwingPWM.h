#pragma once

/******************************************************************
  @file       ReefwingPWM.h
  @brief      Any pin PWM Library. This library is designed to provide 
              'software' PWM output for any digital pin on the AVR
              UNO and Nano.
              
  @author     David Such
  @copyright  Please see the accompanying LICENSE file.

  Code:        David Such
  Version:     1.0.1
  Date:        17/08/23

  1.0.1 Original Release.                         17/08/23

  Credits: - This library is a fork of MD_PWM by MajicDesigns. 

  Important Notes:

  - This library uses AVR TIMER1 or TIMER2 to implement the interrupt 
    driven clock. TIMER0 is used by the Arduino millis() clock, TIMER1 
    is commonly used by the Servo library and TIMER2 by the Tone library. 
    Change USE_TIMER (defined at the top of the header file) to select 
    which timer is enabled in the library code.

  - This library has been tested on Arduino Uno and Nano (ie, 328P processor).
         
******************************************************************/

#include <Arduino.h>

 /******************************************************************
 *
 * Definitions - 
 * 
 ******************************************************************/

#ifndef PWMDEBUG
#define PWMDEBUG 0      ///< 1 turns debug output on
#endif

#ifndef USE_DIRECT_IO
#define USE_DIRECT_IO 1 ///< Use direct port I/O for faster digital writes
#endif

#ifndef USE_TIMER
#define USE_TIMER 2     ///< Set to use hardware TIMER1 or TIMER2 (1 or 2)
#endif

#if PWMDEBUG
#define PWMPRINT(s,v)   do { Serial.print(F(s)); Serial.print(v); } while (false)
#define PWMPRINTX(s,v)  do { Serial.print(F(s)); Serial.print(F("0x")); Serial.print(v, HEX); } while (false)
#define PWMPRINTB(s,v)  do { Serial.print(F(s)); Serial.print(F("0b")); Serial.print(v, BIN); } while (false)
#define PWMPRINTS(s)    do { Serial.print(F(s)); } while (false)
#else
#define PWMPRINT(s,v)
#define PWMPRINTX(s,v)
#define PWMPRINTB(s,v)
#define PWMPRINTS(s)
#endif

/**
 * Core object for the ReefwingPWM library
 */
class ReefwingPWM
{
  public:
    static const uint16_t MAX_FREQUENCY = 100;   ///< the maximum PWM frequency allowed
    static const uint8_t MAX_PWM_PIN = 6;        ///< total number of concurrent PWM pins that can be used

  //--------------------------------------------------------------
  /** \name Class constructor and destructor.
   * @{
   */
  /**
   * Class Constructor
   *
   * Instantiate a new instance of the class.
   *
   * The main function for the core object is to set the internal
   * shared variables and timers to default values.
   * 
   * \param pin pin number for this PWM output.
   */
    ReefwingPWM(uint8_t pin);

  /**
   * Class Destructor.
   *
   * Release any allocated memory and clean up anything else.
   * 
   * If all the instances of the class are closed, then the ISR is
   * disconnected and the timer is stopped.
   */
    ~ReefwingPWM(void);
  /** @} */

  //--------------------------------------------------------------
  /** \name Methods for core object control.
   * @{
   */
  /**
   * Initialize the object.
   *
   * Initialize the object data. This needs to be called during setup() 
   * to set items that cannot be done during object creation.
   * 
   * If this is the first instance of this class, then the ISR code 
   * is initialized and the frequency of the hardware timer is set.
   * Subsequent instances do not affect the timer frequency.
   * 
   * \sa enable()
   * 
   * \param freq the PWM frequency in Hz [0..MAX_FREQUENCY] (first instance only).
   * \return true if the pin was successfully enabled for PWM.
   */
    bool begin(uint16_t freq = MAX_FREQUENCY);

  /**
   * Write PWM output value.
   *
   * Set the PWM output value for the pin. This works like the 
   * standard analogWrite() for hardware enabled PWM - 0% duty cycle
   * is 0, 50% is 127 and 100% is 255.
   *
   * \param duty the PWM duty cycle [0..255].
   */
    void write(uint8_t duty) { _pwmDutySP = duty; }

  /**
   * Disable PWM output for this pin.
   *
   * Stops PWM output for the pin related to this class instance.
   * The pin relinquishes its slot allocated for the ISR [0..MAX_PWM_PINS],
   * which can be reused for another pin if needed.
   * 
   * \sa enable()
   * 
   * \return true if this is the last instance to be disabled
   */
    bool disable(void);

  /**
   * Enables PWM output for this pin.
   *
   * Starts PWM output for the pin related to this class instance.
   * The pin takes the next available slot allocated for the ISR [0..MAX_PWM_PINS].
   * If no slots are available, then the method returns false.
   * 
   * \sa disable()
   * 
   * \return true if the pin was successfully enabled.
   */
    bool enable(void);

  /** @} */
private:
#if USE_TIMER == 1
    static const uint32_t TIMER_RESOLUTION = 65535;    ///< Timer1 is 16 bit

#elif USE_TIMER == 2
    static const uint32_t TIMER_RESOLUTION = 256;    ///< Timer2 is 8 bit

#endif

    uint8_t _pin;                  // PWM digital pin
#if USE_DIRECT_IO
    volatile uint8_t *_outReg;     // output port register for fast writing
    volatile uint8_t _outRegMask;  // output pin in port register for fast writing
#endif
    volatile uint8_t _pwmDuty;     // PWM duty current value point
    volatile uint8_t _pwmDutySP;   // PWM duty setpoint

    void setFrequency(uint32_t freq); // set TIMER frequency
    inline void setTimerMode(void);   // set TIMER mode
    inline void attachISR(void);      // attach to TIMER ISR
    inline void detachISR(void);      // detach from TIMER ISR
    inline void stop(void);           // stop the timer

public:
  static bool _bInitialised;          ///< ISR - Global vector initialization flag
  static uint8_t _cycleCount;         ///< ISR - Global counter
  static ReefwingPWM* _cbInstance[];  ///< ISR - Callback instance handle per pin slot

  //--------------------------------------------------------------
  /** \name ISR use only - NOT FOR END USERS.
   * @{
   */
   /**
   * Set the output pin (for ISR use only).
   *
   * Set the output PWM pin based on the current counter value. This is
   * called 255 times during each PWM cycle.
   * 
   * This method is for use only by the TIMERn ISR.
   */
   void setPin(void);

    /** @} */
};

