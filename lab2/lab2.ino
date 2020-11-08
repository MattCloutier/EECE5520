////////////////////////////////////////////////////////////////////////////////
//
//  Microprocessors II - EECE.5520
//  Lab 2
//
//  Matthew Cloutier
//
//  Filename: lab2.ino
//
//  Purpose: Main module for the Game Control w/ Joystick, Gyro 
//           and Accelerometer lab.
//
//  Notes:
//
////////////////////////////////////////////////////////////////////////////////

#include <MPU6050.h>
#include <Wire.h>
#include <CircularBuffer.h>

// LED status
int incomingByte = 0;

// Joystick Pin Definitions
const int SW_pin = 2; // digital pin connected to switch output
const int X_pin = 0; // analog pin connected to X output
const int Y_pin = 1; // analog pin connected to Y output

int X_val = 0;
int Y_val = 0;

// I2C address for the MPU-6050
const int MPU_addr = 0x68;

// Accelerometer x, y, z variables
int16_t AcX = 0;
int16_t AcY = 0;
int16_t AcZ = 0;


// Gyro x, y, z variables
int16_t GyX = 0;
int16_t GyY = 0;
int16_t GyZ = 0;
int32_t x_delta = 0;
int32_t y_delta = 0;
int16_t xDiff = 5500;
int16_t yDiff = 10000;

// Circular Buffer for the rolling Gyro averages
CircularBuffer<int16_t, 100> rollingGyX;
CircularBuffer<int16_t, 100> rollingGyY;

// Circular Buffer for the rolling accelerometer averages
CircularBuffer<int16_t, 100> rollingAcX;
CircularBuffer<int16_t, 100> rollingAcY;

// Temperature variable
int16_t Temp;

// Active Buzzer Pin definition
const int buzzerPin = 10;    // number of the active buzzer pin
const int buttonPin = 12;    // number of the push-button pin

bool useJoystick = true;

/* 
  Function to wake up and initialize the MPU-6050.
  Seperated to easily enable / disable this functionality.
*/
void setup_MPU(){
  Wire.begin();
  Wire.beginTransmission(MPU_addr);
  Wire.write(0x6B);
  Wire.write(0);
  Wire.endTransmission(true);
}

void setup() {
  // Wake up the MPU-6050
  setup_MPU();
  
  // Initialize serial communication at 9600 bits per second:
  Serial.begin(9600);

  pinMode(buzzerPin, OUTPUT);
  
  // Initialize the push-button pin as an input
  pinMode(buttonPin, INPUT_PULLUP);
}

void checkButton()
{
  if (digitalRead(buttonPin) == LOW)
  {
    // Toggle useJoystick boolean
    useJoystick = !useJoystick;
    Serial.println("toggle");
    
    // Trigger the buzzer so we know to release the button
    digitalWrite(buzzerPin, HIGH);
    delay(2);
    digitalWrite(buzzerPin, LOW);

    delay(1000);
  }
}



/*
  Function: readJoystick()

  Purpose:  Read the digital and analog pins from the joystick,
            and convert that to a WASD direction.

  Notes:    Seperated to easily enable/disable.             
*/
void readJoystick(){

  if (useJoystick)
  {
    // Check the status of the Joystick, and forward to the snake script
    X_val = analogRead(X_pin);
    Y_val = analogRead(Y_pin);
  
    // Determine the direction to move
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
  }
}


void readMPU(){
  
  Wire.beginTransmission(MPU_addr);
  Wire.write(0x3B);  // starting with register 0x3B (ACCEL_XOUT_H)
  Wire.endTransmission(false);

  // Request the data from the MPU
  Wire.requestFrom(MPU_addr,14,true);  // request a total of 14 registers
  AcX=Wire.read()<<8|Wire.read();   // 0x3B (ACCEL_XOUT_H) & 0x3C (ACCEL_XOUT_L)    
  AcY=Wire.read()<<8|Wire.read();   // 0x3D (ACCEL_YOUT_H) & 0x3E (ACCEL_YOUT_L)
  AcZ=Wire.read()<<8|Wire.read();   // 0x3F (ACCEL_ZOUT_H) & 0x40 (ACCEL_ZOUT_L)
  Temp=Wire.read()<<8|Wire.read();  // 0x41 (TEMP_OUT_H) & 0x42 (TEMP_OUT_L)
  GyX=Wire.read()<<8|Wire.read();   // 0x43 (GYRO_XOUT_H) & 0x44 (GYRO_XOUT_L)
  GyY=Wire.read()<<8|Wire.read();   // 0x45 (GYRO_YOUT_H) & 0x46 (GYRO_YOUT_L)
  GyZ=Wire.read()<<8|Wire.read();   // 0x47 (GYRO_ZOUT_H) & 0x48 (GYRO_ZOUT_L)

  // Add the current X and Y gyro values to the rolling buffer
  rollingGyX.push(GyX);
  rollingGyY.push(GyY);
  rollingAcX.push(AcX);
  rollingAcY.push(AcY);
  
  float GyXavg = 0;
  float GyYavg = 0;
  float AcXavg = 0;
  float AcYavg = 0;
  
  // Calculate the rolling averages
  using index_t = decltype(rollingGyX)::index_t;
  for (index_t i = 0; i < rollingGyX.size(); i++){
    GyXavg += rollingGyX[i] / rollingGyX.size();
    GyYavg += rollingGyY[i] / rollingGyY.size();
    
    AcXavg += rollingAcX[i] / rollingAcX.size();
    AcYavg += rollingAcY[i] / rollingAcY.size();    
  }

  // Since we can only move in one direction, see which one has the
  // highest delta from the previous frame.
  x_delta = abs(GyX - (int16_t)GyXavg);
  y_delta = abs(GyY - (int16_t)GyYavg);

  if (!useJoystick)
  {

      if (GyX < -5500)
      {
        // Direction is left
        Serial.println('a');
      }
      else if (GyX > 5500)
      {
        // Direction is right
        Serial.println('d');
      }

     else if (GyY > 0 + yDiff)
      {
        // Direction is up
        Serial.println('w');      
      }
      else if (GyY < 0 - yDiff)
      {
        // Direction is down
        Serial.println('s');
      }

    delay(200);
  }
  
  // Check to see if the acceleration gate is broken
  if ((AcX > 20 * 1300) || (AcY > 6000))
  {
    Serial.println('q');
  }
}

void loop() {

  readJoystick();

  readMPU();
  
  delay(100);

  checkButton();

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
