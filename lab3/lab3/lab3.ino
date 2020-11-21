////////////////////////////////////////////////////////////////////////////////
//
//  Microprocessors II - EECE.5520
//  Lab 3
//
//  Matthew Cloutier
//
//  Filename: lab3.ino
//
//  Purpose: Main module for the Controlling a Fan lab.
//
//  Notes:
//
////////////////////////////////////////////////////////////////////////////////

#include <DS3231.h>
#include "IRremote.h"
#include <LiquidCrystal.h>
#include <TimerOne.h>

#define ENABLE 13
#define DIRA 11
#define DIRB 12
#define SECOND_DELAY 1000

/* Definitions for fan speeds */
#define ZERO 0
#define HALF 128
#define THREE_QUART 192
#define FULL 255

DS3231 clock;
RTCDateTime dt;

String message;

/* Bool to keep track of fan direction. */
bool clockwise = false;

/* short to keep track of fan speed. */
uint8_t rpm = 0;

/* Initialize the library with the LCD pins
 * LCD RS pin to digital pin 53
 * LCD Enable pin to digital pin 51
 * LCD D4 pin to digital pin 44
 * LCD D5 pin to digital pin 45
 * LCD D6 pin to digital pin 46
 * LCD D7 pin to digital pin 47
*/
LiquidCrystal lcd(53, 51, 44, 45, 46, 47);

// Signal Pin of IR receiver to Arduino Digital Pin 24
int receiver = 24;

IRrecv irrecv(receiver);     // create instance of 'irrecv'
decode_results results;      // create instance of 'decode_results'

void setup() {
  //---set pin direction
  pinMode(ENABLE, OUTPUT);
  pinMode(DIRA, OUTPUT);
  pinMode(DIRB, OUTPUT);
  Serial.begin(9600);

  /* Get the current date-time and initialize the RTC */
  clock.begin();
  clock.setDateTime(__DATE__,__TIME__);

  dt = clock.getDateTime();

  /* Setup the LCD's number of columns and rows */
  lcd.begin(16, 2);

  /* Start the receiver. */
  irrecv.enableIRIn();

  Timer1.initialize(1000000); // set a timer of length 1000000 microseconds (or 1 sec)
  Timer1.attachInterrupt( display_message ); // attach the service routine here

}

void display_message()
{
  /* Display the Time on the LCD */
  lcd.setCursor(0, 0);
  lcd.print(dt.hour % 12);
  //lcd.setCursor(2,0);
  lcd.print(':');
  lcd.print(dt.minute);
  lcd.print('.');
  lcd.print(dt.second);

  /* Print the Fan speed. */
  lcd.setCursor(0, 1);
  if (rpm == FULL) {
    lcd.print("FULL");
  }
  else if (rpm == THREE_QUART) {
    lcd.print("3/4 ");
  }
  else if (rpm == HALF){
    lcd.print("HALF");
  }
  else {
    lcd.print("ZERO");
  }

  /* Print the fan direction.  */
  if (clockwise){
    lcd.print("   CW");
  }
  else {
    lcd.print("  CCW");
  }
}

void enable() {
  digitalWrite(ENABLE, LOW); // enable off
  delay(5);

  if (clockwise)
  {
  digitalWrite(DIRA, HIGH);
  digitalWrite(DIRB, LOW);
  }
  else
  {
    digitalWrite(DIRA, LOW);
    digitalWrite(DIRB, HIGH);
  }

  analogWrite(ENABLE, rpm); // enable on
}

void disable() {
  rpm = 0;
  digitalWrite(ENABLE, rpm); // disable
}

void toggleFanDir()
{
  bool wasOn = false;

  /* If the fan is on, turn it off. */
  if (rpm != ZERO)
  {
    wasOn = true;
    disable();
    delay(10);
  }

  if (clockwise)
  {
    digitalWrite(DIRA, LOW);
    digitalWrite(DIRB, HIGH);
    clockwise = false;
  }
  else
  {
    digitalWrite(DIRA, HIGH);
    digitalWrite(DIRB, LOW);
    clockwise = true;
  }

  /* If the fan was previously on, turn it back on now.*/
  if (wasOn)
  {
    enable();
  }
}

void increaseSpeed()
{
  if (rpm == 0)
  {
    rpm = 128;
  }
  else if (rpm == 128)
  {
    rpm = 192;
  }
  else if (rpm == 192)
  {
    rpm = 255;
  }
  else
  {
    /* Already at max, so go back to 0. */
    rpm = 0;
  }

  analogWrite(ENABLE, rpm);
}

void decreaseSpeed()
{
  if (rpm == 128)
  {
    rpm = 0;
  }
  else if (rpm == 192)
  {
    rpm = 128;
  }
  else if (rpm == 255)
  {
    rpm = 192;
  }
  else
  {
    /* Already at min, so go back to 255. */
    rpm = 255;
  }

  analogWrite(ENABLE, rpm);
}

/* Takes action based on IR code received describing Remote IR codes */
void translateIR()
{
  switch(results.value)
  {
  case 0xFFA25D:
    /* Power button was pressed, turn off fan. */
    Serial.println("POWER");
    disable();
    break;
  case 0xFFE21D:
  /* Function button was pressed, toggle fan direction. */
    Serial.println("FUNC/STOP");
    toggleFanDir();
    break;
  case 0xFF629D:
    /* Volume Up button was pressed, increase fan speed */
    Serial.println("VOL+");
    increaseSpeed();
    break;
  case 0xFFA857:
    /* Volume Down button was pressed, decrease fan speed */
    Serial.println("VOL-");
    decreaseSpeed();
    break;
  default:
    Serial.println(" other button   ");
  }

  delay(500); // Do not get immediate repeat

}

void loop()
{
  dt = clock.getDateTime();

  /* have we received an IR signal? */
  if (irrecv.decode(&results))
  {
    translateIR();

    // receive the next value
    irrecv.resume();
  }

}
