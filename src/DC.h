#ifndef DC_H
#define DC_H

#include "pinLibrary.h"
#include <Arduino.h>
#include <math.h>

template <class T> class DC {
    public:
        DC(int *outArray, const size_t &_outSize, int *inArray, const size_t &_inSize);
        ~DC();
        
        // logic setters
        void updateDirection();
        void brake();
        void setDutyStep(const T *value);
        void checkDIR_input();
        void setupOutputPins() const;
        void setupInputPins() const;
        void setOutputPins();
        void checkDirection();
        void updateSpeed();

        // object setters
        void setLedPin(const int& pin);
        void setResolutions(const T *adc, const T *dac); // assume 10-bit AD converter and 8-bit DA converter when not specified

        // getter
        T getDuty() const;

    private:
        // PWM parameters
        T readingLimit = 0;
        T duty = 0;
        T dutyLimit = 0;
        T dutyDesired = 0;
        T dutyStep = 0;

        // motor direction and logic variables
        bool forward = false;
        bool prevForward = false;
        bool right = false;
        bool left = false;
        bool acc = false;
        bool changedDirection = false;
        bool ledState = false;

        size_t outSize = 0;
        size_t inSize = 0;

        int ledPin = 0;

         // pointer to dynamically allocated array used for setting up output pins
        int *H_outputPins;
        
         // pointer to dynamically allocated array used for setting up input pins
         int *arduino_inputPins;      
};

// int* ptrArray[size]; // ptr array
// int* *ptrToFirstElement = ptrArray; // ptr to array of ptrs (to first element)
// // OR
// int** ptrToFirstElement = new int*[size] // ptr to dynamically allocated array of ptrs (ptr to array)

bool getSR_input();

template class DC<unsigned int>;

template class DC<float>;

template class DC<double>;

#endif
