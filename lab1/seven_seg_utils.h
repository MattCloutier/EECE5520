#include <Arduino.h>

/* 
 * Pinout for Lab 1 
 */
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


/*
 * Clear the 7-seg display 
 */
void cathode_high();

/*
 * the 7-segment led display 'P'
 */
void letter_p();

/*
 * the 7-segment led display 'U'
 */
void letter_u();

/*
 * the 7-segment led display 'S'
 */
void letter_s();

/*
 * the 7-segment led display 'H'
 */
void letter_h();

/*
 * The 7-segment led display 0
 */
void zero(); 

/*
 * The 7-segment led display 1
 */
void one(); 

/*
 * The 7-segment led display 2
 */
void two(); 

/*
 * The 7-segment led display 3
*/
void three(); 

/* 
 *  The 7-segment led display 4
 */
void four(); 

/*
 * The 7-segment led display 5
 */
void five(); 

/*
 * The 7-segment led display 6
 */
void six(); 

/*
 * The 7-segment led display 7
 */
void seven(); 

/*
 * The 7-segment led display 8
 */
void eight(); 

/*
 * The 7-segment led display 9
 */
void nine(); 

/*
 * The function is to control the 7-segment LED display to display numbers. 
 */
void displayNumber(int x);

/* Enable one of the digits of the 7-segment display */
void enableDigit(int x);
