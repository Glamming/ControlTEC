#include <eRCaGuy_NewAnalogRead.h>
#include <FastRunningMedian.h>
#include <EEPROM.h>
#include <SoftReset.h>
#include <LiquidCrystal.h>
#include <math.h>
#define THERM_PIN 7    // 10ktherm & 10k resistor as divider.
#define INTERNAL_THERM_PIN 6
#define RELAY_PIN A0  //To reverse the polarity to the peltier chip. LOW = Cold; HIGH = Hot;
#define MOSFET_PIN 11  //Needs pwm
#define FAN_PIN 9  //Needs pwm
#define actionBtn 12
#define upBtn 8
#define downBtn 10
#define maxRampTime 5760 //Time in minutes, 4 days
#define minRampTime 5 // Time in mintutes, 5 min
#define maxNumberOfCycles 20 //


//Oversampliing library
//float analog_reading = adc.analogReadXXbit(pin,bits_of_precision,num_samples); //get the avg. of [num_samples] 16-bit readings
byte bitsOfResolution = 13; //commanded oversampled resolution
unsigned long numSamplesToAvg = 2; //number of samples AT THE OVERSAMPLED RESOLUTION that you want to take and average
ADC_prescaler_t ADCSpeed = ADC_FAST;
 
 
//Maximum operating temperatures
#define maxSafeTemp 80
#define minSafeTemp 12

byte fourHertzCounter = 0;

byte readyToBeginRamp = 0;

float tempCoeff = 0.725;

float lastExternalTemp;
float tempCoeffFudge = 0;

// initialize the library with the numbers of the interface pins
LiquidCrystal lcd(7, 6, 5, 4, 3, 2);  //RS:7 E:6 d4:5 d5:4 d6:3 d7:2  (LCD pins:arduino nano/uno pins)    //LCD has 16 characters over 2 rows
const int whileLoop = 0; //This is to make while loops infinite until the break funtion is called.
int whileLoopCounter = 0; //used to count the number of loops through a while statement.


//Button and button state declarations
//const byte actionBtn = 12;
//const byte upBtn = 8;
//const byte downBtn = 10;
byte actionBtnState = 0;
byte upBtnState = 0;
byte downBtnState = 0;

int btnHoldTime = 0;

float thermostatSetTemperature = 50.0; //Integar used to set the thermostat temperature via the watchdog timer which calls the thermostat function
byte thermostatActive = 0; //Has the user activated the thermostat?

//The array containing menu choices with two variables to control user's position in the menu
char* programMenuChoices[] = {"   Hold Temp", "   Ramp Temp", "   Settings", "Fet Calibration",  " Review Calibr. ", "     Reset"};
const byte programMenuSize = 5;
byte currProgramMenuSel = 0;

//

int holdTempSetTemp = 20; //Contains the desired hold temperature
int rampTempStartTemp = 20; //Contains the desired ramp start temperature
int rampTempEndTemp = 20; //Contains the desired ramp end temperature
int rampTime = 30; //Contains the desired time to complete the temperature ramp
byte numberOfCycles = 1; //Contains the max number of temperature cycles

float externalTemp; //Used to store external thermistor readings
int internalTemp; //Used to store external thermistor readings

//Used to store the calibration settings from the EEPROM memory
int EEPROM_Internal_Temp_Calib;
int EEPROM_External_Temp_Calib;

//Used when calculating the power (in bits) to set the MOSFET to.
int mosfetPower;


void setup() {
  
  
  // initialize Timer1
  cli();          // disable global interrupts
  TCCR1A = 0;     // set entire TCCR1A register to 0
  TCCR1B = 0;     // same for TCCR1B

  // set compare match register to desired timer count:
  OCR1A = 3906.25; //4 Hz
  //OCR1A = 1561.5; // = (16*10^6) / (10*1024) - 1 (must be <65536) 
  //16*10^6 = 16MHz arduino clock, Timer 1 is 16 bit, hence max 65536
  //We want to interupt 10 times a second, hence 10*1024 (1024 = prescaler)
  //See http://www.instructables.com/id/Arduino-Timer-Interrupts/?ALLSTEPS
  
  // turn on CTC mode:
  TCCR1B |= (1 << WGM12);
  // Set CS10 and CS12 bits for 1024 prescaler:
  TCCR1B |= (1 << CS10);
  TCCR1B |= (1 << CS12);
  // enable timer compare interrupt:
  TIMSK1 |= (1 << OCIE1A);
  sei();          // enable global interrupts
  
  
  
  //Oversampling setup
  //Configure the adc how you want it
  adc.setADCSpeed(ADCSpeed);
  adc.setBitsOfResolution(bitsOfResolution);
  adc.setNumSamplesToAvg(numSamplesToAvg);
  
  
  pinMode(actionBtn, INPUT);
  pinMode(upBtn, INPUT);
  pinMode(downBtn, INPUT);
  pinMode(RELAY_PIN, OUTPUT);
  pinMode(MOSFET_PIN, OUTPUT);
  pinMode(FAN_PIN, OUTPUT);
  
    // set up the LCD's number of columns and rows: 
  lcd.begin(16, 2);
  // Print a message to the LCD.
  lcd.setCursor(0, 0);
  lcd.print("Glenn Lamming's");
  lcd.setCursor(0, 1);
  lcd.print("Crystal-O-Matic");
  
  delay(1000);
  
  //Read EEPROM Memory
  EEPROM_Internal_Temp_Calib = EEPROM.read(0) - 100; //I'm setting zero = 100 deg C; normalise for this.
  EEPROM_External_Temp_Calib = EEPROM.read(1) - 100;  
  
  //Set everything switched off/low
  digitalWrite(RELAY_PIN, LOW);
  analogWrite(MOSFET_PIN, 0);
  //analogWrite(FAN_PIN, 255);
  
  Serial.begin(9600);
  lcd.clear();
}



void loop() {
  //Call the initial menu function which contains a while loop that only ends once an option
  //has been selected.
  currProgramMenuSel = 0;
  lcd.clear();
  programMenu();
  
  //Once an option has been selected, programMenu exits and the selected program is executed.
  delay(500);
  if (currProgramMenuSel == 0){
    holdTempMenu();
  } else if (currProgramMenuSel == 1){
    rampTempMenu();
  } else if (currProgramMenuSel == 2){
    settingsMenu();
  
  } else if (currProgramMenuSel == 3){
    fetCalibMenu();
    
  } else if (currProgramMenuSel == 4){
    reviewFetCalib();
    
  } else if (currProgramMenuSel == 5){
    currProgramMenuSel = 0;
    delay(500);
  }
}
