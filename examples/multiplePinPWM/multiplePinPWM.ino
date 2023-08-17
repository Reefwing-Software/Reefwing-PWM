/******************************************************************
  @file       multiplePinPWM.ino
  @brief      Software PWM on 4 pins
  @author     David Such
  @copyright  Please see the accompanying LICENSE file.

  Code:        David Such
  Version:     1.0.1
  Date:        17/08/23

  1.0.1 Original Release.                         17/08/22

******************************************************************/

#include <ReefwingPWM.h>

#define ARRAY_SIZE(a) (sizeof(a)/sizeof((a)[0]))

ReefwingPWM pwm[] = { ReefwingPWM(2), ReefwingPWM(3), ReefwingPWM(4), ReefwingPWM(5) };
const uint16_t PWM_FREQ = 50;               // in Hz

void setup() {
  //  Start Serial and wait for connection
  Serial.begin(115200);
  while (!Serial);

  //  Initialise PWM for each pin
  for (uint8_t i = 0; i < ARRAY_SIZE(pwm); i++) {
    if (pwm[i].begin(PWM_FREQ)) {
      Serial.print("PWM Initialised on Channel: ");
      Serial.println(i);
    }
    else {
      Serial.print("Unable to initialise PWM on Channel: ");
      Serial.println(i);
      while(1);
    }
  }

  //  Set PWM Duty Cycle = 50%. pwm.write(0..255).
  //  0% duty cycle = 0, 50% = 127 and 100% = 255.
  for (uint8_t i = 0; i < ARRAY_SIZE(pwm); i++) {
    pwm[i].write(127)
  }

  //  Wait 5 Seconds
  delay(5000);

  //  Turn off PWM
  for (uint8_t i = 0; i < ARRAY_SIZE(pwm); i++) {
    pwm[i].disable();
  }
}

void loop() { }