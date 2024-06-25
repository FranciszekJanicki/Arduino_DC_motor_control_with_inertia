#include "DC.h"

template <class T>
DC<T>::DC(const int outArray[], const size_t& _outSize, const int inArray[], const size_t& _inSize) : outSize(_outSize), inSize(_inSize) {

    // setting up an object, nothing fancy here

    H_outputPins = new int[outSize];
    for (size_t i = 0; i < inSize; ++i) {
        H_outputPins[i] = outArray[i];
    }

    arduino_inputPins = new int[inSize];
    for (size_t i = 0; i < inSize; ++i) {
        arduino_inputPins[i] = inArray[i];
    }
}

template <class T>
DC<T>::~DC() {
    delete[] H_outputPins;
    delete[] arduino_inputPins;
}

template <class T>
void DC<T>::setDutyStep(const T* value) {
    dutyStep = *value;
}

// setting up output pins
template <class T>
void DC<T>::setupOutputPins() const {
    size_t size = sizeof(H_outputPins) / sizeof(H_outputPins[0]);

    for (size_t i = 0; i < size; ++i) {

        //if (H_outputPins[i] == nullptr) {
          //  char error[] = "Null pointer encountered!";
         //   throw error;

        //} else {
            pinMode(H_outputPins[i], OUTPUT);
            digitalWrite(H_outputPins[i], false);
       // }

    }
}

// setting up input pins
template <class T>
void DC<T>::setupInputPins() const {
    size_t size = sizeof(arduino_inputPins) / sizeof(arduino_inputPins[0]);

    for (size_t i = 0; i < size; ++i) {

        //if (arduino_inputPins[i] == nullptr) {
            //char error[] = "Null pointer encountered!";
            //throw error;

        //} else {
            pinMode(arduino_inputPins[i], INPUT_PULLUP);
       // }

    }
}

template <class T>    
void DC<T>::setLedPin(const int& pin) {

    //if (pin == nullptr) {
        //char error[] = "Null pointer encountered!";
        //throw error;
        
    //} else {
        ledPin = pin;
        pinMode(ledPin, OUTPUT);
        digitalWrite(ledPin, INPUT_PULLUP);
    //}
}

// set dutyLimit (duty==dutyLimit means 100% PWM duty) and readingLimit (reading==readingLimit means 5V, max reading)
template <class T>
void DC<T>::setResolutions(const T* adc, const T* dac) {
    dutyLimit = pow(2, *dac) - 1;
    readingLimit = pow(2, *adc) - 1;
}

// seting values of pins connected to H-bridge 
template <class T>
void DC<T>::setOutputPins() {

    analogWrite(H_outputPins[0], duty);
    digitalWrite(H_outputPins[1], left);
    digitalWrite(H_outputPins[2], right);
    digitalWrite(ledPin, ledState);
}

template <class T>
void DC<T>::updateDirection() {
    // variables left and right depend of forward variable
    // you could ask why not just use forward variable, (forward instead of right and !left and !forward instead of left and !right), 
    // well I want to be able to assign both of them to the same values to be able to brake my motor!
    if (forward) {
        right = true;
        left = false;
    } else {
        right = false;
        left = true;
    }
}

// this is self explanatory
template <class T>
void DC<T>::brake() {
    left = true;
    right = true;
    dutyDesired = 0;
}

// logic for changing direction and first decelerating in old direction, then after stopping accelerating in new direction (see README!!!)
template <class T>
void DC<T>::checkDirection() {

    if (forward != prevForward) { // change of direction needed
        changedDirection = false; // start procedure of changing direction, so you didnt changed yet
    }

    if (!changedDirection) {
        dutyDesired = 0; // decelerate to 0 for changing direction
    } else {
        updateDirection(); // then after changing direction (with my bool variable) Im doing it 'physically'
        // dutyDesired = map(analogRead(potentiometer), 0, readingLimit, 0, dutyLimit); // set new speed to reading value (mapping the ADC reading to DAC reading)
        dutyDesired = map(analogRead(arduino_inputPins[1]), 0, readingLimit, 0, dutyLimit); 
    }
}

// self-explanatory
template <class T>
void DC<T>::updateSpeed() {

    if (duty > dutyDesired) {
        acc = false;
    } else if (duty < dutyDesired) {
        acc = true;
    }

    if (acc) {
        if (duty < dutyLimit) duty += dutyStep;
    } else {
        if (duty > dutyLimit) duty -= dutyStep;
        else changedDirection = true; // duty reached 0 then it's time to change direction
    }
}

// some basic getters and setters for better looking code
template <class T>
T DC<T>::getDuty() const {
    return duty;
}

// method for checking digital input and setting up logic variables on the go
template <class T>
void DC<T>::checkDIR_input() {

    // static variable to retain values between calls (static variables are initialized only once, thus below line will only execute once!)  
    static bool DIR_prevState = false;

    // bool DIR_state = !digitalRead(DIR_BUTTON);
    bool DIR_state = !digitalRead(arduino_inputPins[0]);

    ledState = false;

    if (DIR_state && !DIR_prevState) {
        ledState = true;
        forward = !forward;
    }

    DIR_prevState = DIR_state;
}

// function for checing digital input (SR = SET/RESET = ON/OFF)
bool getSR_input() {
    bool SR = false;
    static bool SR_prevState = false;
    bool SR_state = !digitalRead(SR_BUTTON);

    if (SR_state && !SR_prevState) {
        SR = !SR;
    }

    SR_prevState = SR_state;

    return SR;
}