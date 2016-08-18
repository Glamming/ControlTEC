# ControlTEC
Peltier (TEC) controller with fine termperature control and slow temperature ramping

## Notes about software
- Code tested using the Arduino IDE 1.6.9 - https://www.arduino.cc/en/Main/Software
- Code written for Arduino Nano v3 and uploaded using AVRISP mkII programmer, part of the Arduino IDE
- The Arduino Nano's ADC circuitry are only 10 bit, to achieve a temperature resolution of +/- 0.1 C, the ADC's are oversampled to improve their theoretical limit to 13 bit.

## Hardware setup
- Arduino nano v3
- Pin A0 - Connected to a 2N transistor used to switch a relay, H-bridge, reverse polarity across the peltier
- Pin 6 - Reference temperature thermistor (10k therm with 10k resistor as divider)
- Pin 7 - Peltier chip thermistor (10k therm with 10k resistor as divider)
- Pin 8 - User interface button
- Pin 9 - PWM pin to control fan speed
- Pin 10 - User interface button
- Pin 11 - PWM pin connected to the gate of MOSFET used to regulate the current flow to the peltier chip
- Pin 12 - User interface button
- Pins 7, 6, 5, 4, 3, 2 - Used to drive the LCD screen


## Required libraries
### External libraries
- eRCaGuy_NewAnalogRead.h 2.1 alpha (GPLV3) - http://www.electricrcaircraftguy.com/2014/05/using-arduino-unos-built-in-16-bit-adc.html
- FastRunningMedian.h 0.1.01 - http://forum.arduino.cc/index.php?topic=53081

### Inclluded as part of the Arduino IDE 1.6.9
- EEPROM.h
- LiquidCrystal.h
- math.h
