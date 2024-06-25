#include "DC.h"

// array of macros's of led pin numbers -> see pinLibrary.h file
const int ledPins[] = {
  LED_SR, // used to sygnalize the devices power supply
  LED_DIR // used to blink when button for changing direction value
};


// initializing parameters for the motor:

// 1. Create an array of macros for H-bridge pins (standard notation for most full H-bridges)
// array of macros of input pin numbers -> see pinLibrary.h file
const int inputPins[] = {
    DIR_BUTTON,
    potentiometer
};
size_t sizeInput = sizeof(inputPins) / sizeof(inputPins[0]);

// array of macros of output pin numbers -> see pinLibrary.h file
const int outputPins[] = {
  enable, 
  input1, 
  input2
};
size_t sizeOutput = sizeof(outputPins) / sizeof(outputPins[0]);


// 2. Create ptr of object type
DC<unsigned int>* motor;

// 3. Set parameters of your board's ADC and DAC bit resoluations
const unsigned int DACresolution = 8;
const unsigned int ADCresolution = 10;
const unsigned int dutyStep = 1;


void setup() {
  // allocate memory to ptr
  motor = new DC<unsigned int>(outputPins, sizeOutput, inputPins, sizeInput);

  // 4. Call OPTIONAL setters
  motor->setResolutions(&ADCresolution, &DACresolution);
  motor->setDutyStep(&dutyStep);
  motor->setLedPin(LED_DIR);



  pinMode(SR_BUTTON, INPUT_PULLUP);
    
  pinMode(LED_SR, OUTPUT);
  digitalWrite(LED_SR, false);

}

// main logic
bool SR = false;

void loop() {

  // function for handling digital input on SR pin (SR = SET/RESET = ON/OFF)
  SR = getSR_input();
  
  // main program logic
  if (SR) {

    // method for handling digital input on DIR pin (changing direction)
    motor->checkDIR_input();

    // methods for handling motor
    motor->checkDirection();
    motor->updateSpeed();

  } else {

    // brake with current
    motor->brake();
  }

    // actualize H-bridge pins and LED_DIR based on values calculated in above methods
    motor->setOutputPins();

    // actualize LED_SR pin based on SR value
    digitalWrite(LED_SR, SR);
}
