/******************************************************************
  @file       simplePWM.ino
  @brief      Example demonstrating ReefwingPWM functionality
  @author     David Such
  @copyright  Please see the accompanying LICENSE file.

  Code:        David Such
  Version:     1.0.0
  Date:        17/08/23

  1.0.0 Original Release.                         17/08/22

******************************************************************/

#include <ReefwingPWM.h>

ReefwingPWM pwm[] = { ReefwingPWM(2) };
const uint16_t PWM_FREQ = 50;               // in Hz

void setup() {
  //  Start Serial and wait for connection
  Serial.begin(115200);
  while (!Serial);

  //  Initialise PWM for each pin
  if (pwm[0].begin(PWM_FREQ)) {
    Serial.println("PWM on Channel 0 Initialised");
  }
  else {
    Serial.println("Unable to initialise PWM on Channel 0");
    while(1);
  }

  //  Set PWM Duty Cycle = 50%. pwm.write(0..255).
  //  0% duty cycle = 0, 50% = 127 and 100% = 255.
  pwm[0].write(127)

  //  Wait 5 Seconds
  delay(5000);

  //  Turn off PWM
  pwm[0].disable();
}

void loop() { }