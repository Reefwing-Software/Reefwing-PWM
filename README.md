[![arduino-library-badge](https://www.ardu-badge.com/badge/ReefwingPWM.svg?)](https://www.ardu-badge.com/Reefwing-PWM) ![version](https://img.shields.io/github/v/tag/Reefwing-Software/Reefwing-PWM) ![license](https://img.shields.io/badge/license-GPL-green) ![release](https://img.shields.io/github/release-date/Reefwing-Software/Reefwing-PWM?color="red") ![open source](https://badgen.net/badge/open/source/blue?icon=github)

# Reefwing PWM

This library is a fork of [MD_PWM](https://github.com/MajicDesigns/MD_PWM/blob/main/README.md) by [MajicDesigns](https://arduinoplusplus.wordpress.com/). Apart from cosmetic changes, the main difference is to increase the maximum number of concurrent PWM pins to 6 (`MAX_PWM_PIN`), and to reduce the maximum PWM Frequency (`MAX_FREQUENCY`) to 100 Hz.

# Library for AVR PWM on arbitrary pins

This library is designed to provide 'software' PWM output for any digital pin.

## Important Notes:
- This library uses AVR `TIMER1` or `TIMER2` to implement the interrupt driven clock, selectable in the library header file. The library defaults to `TIMER2`.
- This library has been tested on Arduino Uno and Nano only (i.e., Arduino boards with the ATMega328P processor).

## Implementation

This library implements user defined frequency PWM output for any digital pin 
software limited to `MAX_FREQUENCY` Hz.

The `TIMERn` is set for 255 times this frequency (eg, 200Hz becomes 51kHz). This 
causes the `TIMERn` interrupt routine to be called 255 times for each PWM cycle 
and, depending on where it is in the cycle, allows the software to set the 
digital output pin to `LOW` or `HIGH`, thus creating the desired PWM signal. 

The duty cycle can be changed smoothly by changing the set point at which 
the digital transition occurs. The new duty cycle takes effect at the start 
of the next PWM transition.

`TIMERn` is a global resource, so each object instance of class is driven from the
same `TIMERn` interrupt. The constant `MAX_PWM_PIN` is used to set limits the
global maximum for instances allowed to be processed by the interrupt.

