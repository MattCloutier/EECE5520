////////////////////////////////////////////////////////////////////////////////
//
//  Microprocessors II - EECE.5520
//  Lab 2
//
//  Matthew Cloutier
//
//  Filename: lab2.ino
//
//  Purpose: Main module for the
//
//  Notes:
//
////////////////////////////////////////////////////////////////////////////////

// LED status
int incomingByte = 0;

// Joystick Pin Definitions
const int SW_pin = 2; // digital pin connected to switch output
const int X_pin = 0; // analog pin connected to X output
const int Y_pin = 1; // analog pin connected to Y output

int X_val = 0;
int Y_val = 0;

// Active Buzzer Pin definition
const int buzzerPin = 10;    // number of the active buzzer pin

void setup() {
  // initialize serial communication at 9600 bits per second:
  Serial.begin(9600);

  pinMode(buzzerPin, OUTPUT);

}

void loop() {

  /* Check the status of the Joystick, and forward to the snake script
  Serial.print("Switch:  ");
  Serial.print(digitalRead(SW_pin));
  Serial.print("\n");
  Serial.print("X-axis: ");
  Serial.print(analogRead(X_pin));
  Serial.print("\n");
  Serial.print("Y-axis: ");
  Serial.println(analogRead(Y_pin));
  Serial.print("\n\n");
  */

  // Determine the direction to move
  X_val = analogRead(X_pin);
  Y_val = analogRead(Y_pin);

  if (X_val < 128) {
    // Direction is down
    Serial.println('s');
  }
  else if (X_val > 896) {
    // Direction is up
    Serial.println('w');
  }
  else if (Y_val < 128) {
    // Direction is right
    Serial.println('d');
  }
  else if (Y_val > 896) {
    // Direction is left
    Serial.println('a');
  }

  delay(100);

  // read from the Serial port:
  if (Serial.available() > 0) {
    // read the incoming byte:
    incomingByte = Serial.read();

    if(incomingByte == 'A') {
      digitalWrite(buzzerPin, HIGH);
      delay(2);
      digitalWrite(buzzerPin, LOW);
    }
  }

}
