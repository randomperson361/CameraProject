#include <Arduino.h>
#include "Exposure.h"

// Define pins
#define LIGHT_METER_PIN		A4
#define SELECT_BUTTON_PIN	1
#define BACK_BUTTON_PIN		A5
#define UP_BUTTON_PIN		11
#define DOWN_BUTTON_PIN		12
#define SHUTTER_BUTTON_PIN	0
#define SHUTTER_CONTROL_PIN 13
#define DOT_PIN				9
#define LED_PIN				10

// Define tube letters
#define TUBE_OFF			10
#define TUBE_O				0
#define TUBE_S				5
#define TUBE_T				7
#define TUBE_G				9
#define TUBE_E				11
#define TUBE_C				12
#define TUBE_v				13

// Define tube stuff
/*
  Notes:
  * Macros HIGH and LOW are always passed as arguments of type byte (uint8_t).
  * Pins 2..9 are connected to the segments and dots of the tubes.
  * Pins A0..A3 are connected to the grids of the tubes.

       pin 2
        ---
 pin 7 |   | pin 3
       |   |
 pin 8  ---
       |   | pin 4
 pin 6 |   |
        --- . pin 9
       pin 5
*/

static byte tubeChar[14*7] PROGMEM =
{
// pin    2     3     4     5     6     7     8
        HIGH, HIGH, HIGH, HIGH, HIGH, HIGH,  LOW,	// 0/TUBE_O
         LOW, HIGH, HIGH,  LOW,  LOW,  LOW,  LOW,	// 1
        HIGH, HIGH,  LOW, HIGH, HIGH,  LOW, HIGH,	// 2
        HIGH, HIGH, HIGH, HIGH,  LOW,  LOW, HIGH,	// 3
         LOW, HIGH, HIGH,  LOW,  LOW, HIGH, HIGH,	// 4
        HIGH,  LOW, HIGH, HIGH,  LOW, HIGH, HIGH,	// 5/TUBE_S
        HIGH,  LOW, HIGH, HIGH, HIGH, HIGH, HIGH,	// 6
        HIGH, HIGH, HIGH,  LOW,  LOW,  LOW,  LOW,	// 7/TUBE_T
        HIGH, HIGH, HIGH, HIGH, HIGH, HIGH, HIGH,	// 8
        HIGH, HIGH, HIGH, HIGH,  LOW, HIGH, HIGH,	// 9/TUBE_G
         LOW,  LOW,  LOW,  LOW,  LOW,  LOW,  LOW,  	// TUBE_OFF
        HIGH,  LOW,  LOW, HIGH, HIGH, HIGH, HIGH,	// TUBE_E
        HIGH,  LOW,  LOW, HIGH, HIGH, HIGH,  LOW,	// TUBE_C
         LOW, HIGH, HIGH, HIGH, HIGH, HIGH,  LOW	// TUBE_V
};

typedef  struct  _TUBE
{
	byte    digit;      // 0..9, or letter macros
	byte    dot;        // HIGH or LOW
	byte	led;		// HIGH or LOw
} TUBE;

static TUBE tubeData[4] =
{
	{ TUBE_OFF, LOW, LOW },
	{ TUBE_OFF, LOW, LOW },
	{ TUBE_OFF, LOW, LOW },
	{ TUBE_OFF, LOW, LOW }
};

// Define global variables
static byte currentTube = 3;    // 0..3
Exposure ExpoData(LIGHT_METER_PIN);

void checkButtons()
{

}

void updateNextTube()
{
	byte    *digit_seg_p;
	byte    digit;
	byte    pin;

	digitalWrite(A0+currentTube,LOW);					// turn off the old tube
	currentTube++;										// move to next tube
	currentTube %= 4;									// roll over if needed

	digit = tubeData[currentTube].digit;
	digit_seg_p = tubeChar + 7*digit;
	for (pin = 2; pin < 9; pin++, digit_seg_p++) 		// light needed segments
	{
		digitalWrite(pin,pgm_read_byte(digit_seg_p));
	}
	digitalWrite(DOT_PIN,tubeData[currentTube].dot); 	// turn dot on or off
	digitalWrite(LED_PIN,tubeData[currentTube].led);	// turn the led on of off
	digitalWrite(A0+currentTube,HIGH);					// enable the current tube
}

void initiatePins()
{
	  pinMode(A0,OUTPUT);
	  pinMode(A1,OUTPUT);
	  pinMode(A2,OUTPUT);
	  pinMode(A3,OUTPUT);
	  pinMode(2,OUTPUT);
	  pinMode(3,OUTPUT);
	  pinMode(4,OUTPUT);
	  pinMode(5,OUTPUT);
	  pinMode(6,OUTPUT);
	  pinMode(7,OUTPUT);
	  pinMode(8,OUTPUT);
	  pinMode(9,OUTPUT);
	  pinMode(10,OUTPUT);
	  pinMode(LIGHT_METER_PIN,INPUT_PULLUP);
	  pinMode(SELECT_BUTTON_PIN,INPUT_PULLUP);
	  pinMode(BACK_BUTTON_PIN,INPUT_PULLUP);
	  pinMode(UP_BUTTON_PIN,INPUT_PULLUP);
	  pinMode(DOWN_BUTTON_PIN,INPUT_PULLUP);
	  pinMode(SHUTTER_BUTTON_PIN,INPUT_PULLUP);
	  pinMode(SHUTTER_CONTROL_PIN,OUTPUT);
}

void setup()
{
	analogReference(EXTERNAL);		// TODO: for light sensor, wire 3.3v to AREF, this must be called so that 3.3v is not shorted to 5v
	initiatePins();
	Serial.begin(9600);
}

void loop()
{
	checkButtons();
	ExpoData.updateExposure();
	updateNextTube();	 			// enable this when tubes are actuall connected
	delay(2);			 			// Delay 2 ms => at most 500 cycles/second
}
