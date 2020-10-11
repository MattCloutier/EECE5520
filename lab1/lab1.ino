#include <TimerOne.h>
#include "seven_seg_utils.h"

// Type definitions for this Lab
enum State {
  INIT,
  RED,
  GREEN,
  YELLOW
};

// Definitions for the I/O pins
const int buttonPin = 12;    // number of the push-button pin

const int buzzerPin = 10;    // number of the active buzzer pin

const int redLEDPin = 7;    // number of the red LED pin
const int yellowLEDPin = 6; // number of the yellow LED pin
const int greenLEDPin = 5;   // number of the green LED pin


// Other globals
int initLEDState = LOW;     // variable for toggling the red LED during Init
int counter = 0;            // Number of senconds remaining for the current state
int nextCounter = 0;        // Number of seconds to stay in the next state
int last_counter = 0;       // Last value of the counter. To track time change during the init state.

int digits[4] = {0};        // 4 digit number to display on 7-seg

State currentState = INIT;     // Current state value
State nextState = RED;         // Next state value

void setup() {
  // Initialize the LED pins and the buzzer as an outputs
  pinMode(redLEDPin, OUTPUT);
  pinMode(yellowLEDPin, OUTPUT);
  pinMode(greenLEDPin, OUTPUT);
  pinMode(buzzerPin, OUTPUT);

  // Initialize the push-button pin as an input
  pinMode(buttonPin, INPUT);

  // Initialize the 7-Segment Display
  pinMode(a, OUTPUT);
  pinMode(b, OUTPUT);
  pinMode(c, OUTPUT);
  pinMode(d, OUTPUT);
  pinMode(e, OUTPUT);
  pinMode(f, OUTPUT);
  pinMode(g, OUTPUT);
  pinMode(p, OUTPUT);
  pinMode(d4, OUTPUT);
  pinMode(d3, OUTPUT);
  pinMode(d2, OUTPUT);
  pinMode(d1, OUTPUT);
  
  initDisplay();

  Timer1.initialize(1000000); // set a timer of length 1000000 microseconds (or 1 sec)
  Timer1.attachInterrupt( dec_counter ); // attach the service routine here
 
}

void dec_counter()
{
  counter--;

  if (currentState != INIT && counter > 0 && counter <= 3) {
    digitalWrite(buzzerPin, HIGH);
    delay(5);
    digitalWrite(buzzerPin, LOW);
  }
  
}


void initDisplay()
{

  digits[0] = 11;
  digits[1] = 12;
  digits[2] = 13;
  digits[3] = 14;
  
  // Display the initial message on the 7-segment dispay
  for (int cur_digit = 0; cur_digit < 4; cur_digit++)
  {
    // Clear the display first
    cathode_high();

    enableDigit(cur_digit);

    displayNumber(digits[cur_digit]);

    // Give the display a chance to update.
    delay(5);
  }
}

/* Display a number on the seven segment display */
void Update_SevenSeg()
{
  // Only use last 2 digits for this lab
  digits[0] = counter / 1000;
  digits[1] = (counter % 1000) / 100;
  digits[2] = (counter % 100) / 10;
  digits[3] = counter % 10;

  for (int cur_digit = 0; cur_digit < 4; cur_digit++)
  {
    // Clear the display first
    cathode_high();

    enableDigit(cur_digit);

    displayNumber(digits[cur_digit]);

    // Give the display a chance to update.
    delay(5);
  }
}

void loop() {

  switch (currentState) {
    case RED:
      digitalWrite(redLEDPin, HIGH);
      nextState = GREEN;
      nextCounter = 15;
      break;
    case GREEN:
      digitalWrite(greenLEDPin, HIGH);
      nextState = YELLOW;
      nextCounter = 3;
      break;
    case YELLOW:
      digitalWrite(yellowLEDPin, HIGH);
      nextState = RED;
      nextCounter = 15;
      break;
    default:
      // This is the init state, so check for a button press
      // or toggle the red LED
      if (digitalRead(buttonPin) == HIGH) {
        currentState = nextState;
        counter = 15;
      }
      else if (counter != last_counter) {
        // A second has gone by, so toggle the LED.
        if (initLEDState == LOW) {
          initLEDState = HIGH;
        }
        else {
          initLEDState = LOW;
        }
        digitalWrite(redLEDPin, initLEDState);  
        last_counter = counter;      
      }
      else {    
       initDisplay();
      }
      break;
  }

  // If we have gotten out of the init state,
  // decrement the counter and see if we have any actions to make
  if (currentState != INIT) { 
    Update_SevenSeg();
    
    if (counter == 0) {
      currentState = nextState;
      counter = nextCounter;

      digitalWrite(redLEDPin, LOW);
      digitalWrite(greenLEDPin, LOW);
      digitalWrite(yellowLEDPin, LOW);
    }
  }
}
