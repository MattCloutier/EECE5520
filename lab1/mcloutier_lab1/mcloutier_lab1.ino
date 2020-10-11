#include <TimerOne.h>

// Type definitions for this Lab
enum State {
  INIT,
  RED,
  GREEN,
  YELLOW
};

byte seven_seg_digits[10] = { B11111100,  // = 0
                              B01100000,  // = 1
                              B11011010,  // = 2
                              B11110010,  // = 3
                              B01100110,  // = 4
                              B10110110,  // = 5
                              B10111110,  // = 6
                              B11100000,  // = 7
                              B11111110,  // = 8
                              B11100110   // = 9
                            };

// Definitions for the I/O pins
const int buttonPin = 12;    // number of the push-button pin

const int buzzerPin = 10;    // number of the active buzzer pin

const int redLEDPin = 7;    // number of the red LED pin
const int yellowLEDPin = 6; // number of the yellow LED pin
const int greenLEDPin = 5;   // number of the green LED pin

const int a = 41;
const int b = 33;
const int c = 36;
const int d = 40;
const int e = 42;
const int f = 39;
const int g = 34;
const int p = 38;
const int d4 = 32;
const int d3 = 37;
const int d2 = 35;
const int d1 = 43;

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

/* Clear the 7-seg display */
void cathode_high()
{
  digitalWrite(a, LOW);
  digitalWrite(b, LOW);
  digitalWrite(c, LOW);
  digitalWrite(d, LOW);
  digitalWrite(e, LOW);
  digitalWrite(f, LOW);
  digitalWrite(g, LOW);
  digitalWrite(p, LOW); 
}

void letter_p()
{
  digitalWrite(a, HIGH);
  digitalWrite(b, HIGH);
  digitalWrite(c, LOW);
  digitalWrite(d, LOW);
  digitalWrite(e, HIGH);
  digitalWrite(f, HIGH);
  digitalWrite(g, HIGH);
  digitalWrite(p, LOW);
}

void letter_u()
{
  digitalWrite(a, LOW);
  digitalWrite(b, HIGH);
  digitalWrite(c, HIGH);
  digitalWrite(d, HIGH);
  digitalWrite(e, HIGH);
  digitalWrite(f, HIGH);
  digitalWrite(g, LOW);
  digitalWrite(p, LOW);  
}

void letter_s()
{
  digitalWrite(a, HIGH);
  digitalWrite(b, LOW);
  digitalWrite(c, HIGH);
  digitalWrite(d, HIGH);
  digitalWrite(e, LOW);
  digitalWrite(f, HIGH);
  digitalWrite(g, HIGH);
  digitalWrite(p, LOW);  
}

void letter_h()
{
  digitalWrite(a, LOW);
  digitalWrite(b, HIGH);
  digitalWrite(c, HIGH);
  digitalWrite(d, LOW);
  digitalWrite(e, HIGH);
  digitalWrite(f, HIGH);
  digitalWrite(g, HIGH);
  digitalWrite(p, LOW);  
}

void zero() //the 7-segment led display 0
{
  digitalWrite(a, HIGH);
  digitalWrite(b, HIGH);
  digitalWrite(c, HIGH);
  digitalWrite(d, HIGH);
  digitalWrite(e, HIGH);
  digitalWrite(f, HIGH);
  digitalWrite(g, LOW);
}

void one() //the 7-segment led display 1
{
  digitalWrite(a, LOW);
  digitalWrite(b, HIGH);
  digitalWrite(c, HIGH);
  digitalWrite(d, LOW);
  digitalWrite(e, LOW);
  digitalWrite(f, LOW);
  digitalWrite(g, LOW);
}

void two() //the 7-segment led display 2
{
  digitalWrite(a, HIGH);
  digitalWrite(b, HIGH);
  digitalWrite(c, LOW);
  digitalWrite(d, HIGH);
  digitalWrite(e, HIGH);
  digitalWrite(f, LOW);
  digitalWrite(g, HIGH);
}
void three() //the 7-segment led display 3
{
  digitalWrite(a, HIGH);
  digitalWrite(b, HIGH);
  digitalWrite(c, HIGH);
  digitalWrite(d, HIGH);
  digitalWrite(e, LOW);
  digitalWrite(f, LOW);
  digitalWrite(g, HIGH);
}

void four() //the 7-segment led display 4
{
  digitalWrite(a, LOW);
  digitalWrite(b, HIGH);
  digitalWrite(c, HIGH);
  digitalWrite(d, LOW);
  digitalWrite(e, LOW);
  digitalWrite(f, HIGH);
  digitalWrite(g, HIGH);
}

void five() //the 7-segment led display 5
{
  digitalWrite(a, HIGH);
  digitalWrite(b, LOW);
  digitalWrite(c, HIGH);
  digitalWrite(d, HIGH);
  digitalWrite(e, LOW);
  digitalWrite(f, HIGH);
  digitalWrite(g, HIGH);
}

void six() //the 7-segment led display 6
{
  digitalWrite(a, HIGH);
  digitalWrite(b, LOW);
  digitalWrite(c, HIGH);
  digitalWrite(d, HIGH);
  digitalWrite(e, HIGH);
  digitalWrite(f, HIGH);
  digitalWrite(g, HIGH);
}

void seven() //the 7-segment led display 7
{
  digitalWrite(a, HIGH);
  digitalWrite(b, HIGH);
  digitalWrite(c, HIGH);
  digitalWrite(d, LOW);
  digitalWrite(e, LOW);
  digitalWrite(f, LOW);
  digitalWrite(g, LOW);
}

void eight() //the 7-segment led display 8
{
  digitalWrite(a, HIGH);
  digitalWrite(b, HIGH);
  digitalWrite(c, HIGH);
  digitalWrite(d, HIGH);
  digitalWrite(e, HIGH);
  digitalWrite(f, HIGH);
  digitalWrite(g, HIGH);
}

void nine() //the 7-segment led display 9
{
  digitalWrite(a, HIGH);
  digitalWrite(b, HIGH);
  digitalWrite(c, HIGH);
  digitalWrite(d, HIGH);
  digitalWrite(e, LOW);
  digitalWrite(f, HIGH);
  digitalWrite(g, HIGH);
}

//The function is to control the 7-segment LED display to display numbers. Here x is the number to be displayed. It is an integer from 0 to 9 
void displayNumber(int x)
{
  switch(x)
  {
    default: 
      zero(); 
      break;
    case 1: 
      one(); 
      break;
    case 2: 
      two(); 
      break;
    case 3: 
      three(); 
      break;
    case 4: 
      four(); 
      break;
    case 5: 
      five(); 
      break;
    case 6: 
      six(); 
      break;
    case 7: 
      seven(); 
      break;
    case 8: 
      eight(); 
      break;
    case 9: 
      nine(); 
      break;
    case 11:
      letter_p();
      break;
    case 12:
      letter_u();
      break;
    case 13:
      letter_s();
      break;
    case 14:
      letter_h();
      break;
  }
} 

void enableAllDigits()
{
  digitalWrite(d1, LOW);
  digitalWrite(d2, LOW);
  digitalWrite(d3, LOW);
  digitalWrite(d4, LOW);
}

/* Enable one of the digits of the 7-segment display */
void enableDigit(int x) 
{
  //The 7-segment LED display is a common-cathode one. So also use digitalWrite to  set d1 as high and the LED will go out
  digitalWrite(d1, HIGH);
  digitalWrite(d2, HIGH);
  digitalWrite(d3, HIGH);
  digitalWrite(d4, HIGH);

  switch(x)
  {
    case 0: 
      digitalWrite(d1, LOW);//Light d1 up 
      break;
    case 1: 
      digitalWrite(d2, LOW); //Light d2 up 
      break;
    case 2: 
      digitalWrite(d3, LOW); //Light d3 up 
      break;
    default: 
      digitalWrite(d4, LOW); //Light d4 up 
      break;
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

    enableAllDigits();
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

    enableAllDigits();
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
