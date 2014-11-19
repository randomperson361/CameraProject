/*
 * VFDShield.cpp
 *
 *  Created on: Nov 18, 2014
 *      Author: Andrew
 */

#include "Arduino.h"
#include "VFDShield.h"

const uint8_t VFDShield::segData[] PROGMEM =
{
/* pin    2     3     4     5     6     7     8  */
        HIGH, HIGH, HIGH, HIGH, HIGH, HIGH,  LOW,
         LOW, HIGH, HIGH,  LOW,  LOW,  LOW,  LOW,
        HIGH, HIGH,  LOW, HIGH, HIGH,  LOW, HIGH,
        HIGH, HIGH, HIGH, HIGH,  LOW,  LOW, HIGH,
         LOW, HIGH, HIGH,  LOW,  LOW, HIGH, HIGH,
        HIGH,  LOW, HIGH, HIGH,  LOW, HIGH, HIGH,
        HIGH,  LOW, HIGH, HIGH, HIGH, HIGH, HIGH,
        HIGH, HIGH, HIGH,  LOW,  LOW,  LOW,  LOW,
        HIGH, HIGH, HIGH, HIGH, HIGH, HIGH, HIGH,
        HIGH, HIGH, HIGH, HIGH,  LOW, HIGH, HIGH
};

VFDShield::VFDShield()
{
	currentTube = 0;
	for (int i = 0; i<4; i++)
	{
		digit[i] = ' ';
		dot[i] = LOW;
		led[i] = LOW;
	}
}

VFDShield::~VFDShield()
{
	// TODO Auto-generated destructor stub
}

uint8_t VFDShield::getSegData(uint8_t character, uint8_t seg)
{
	switch (character)
	{
	case '0':
	case 'o':
		uint8_t segData[7] = {HIGH, HIGH, HIGH, HIGH, HIGH, HIGH,  LOW};
		break;
		/*
	case '1':
		segData = {LOW, HIGH, HIGH,  LOW,  LOW,  LOW,  LOW};
		break;
	case '2':
		segData = {HIGH, HIGH,  LOW, HIGH, HIGH,  LOW, HIGH};
		break;
	case '3':
		segData = {HIGH, HIGH, HIGH, HIGH,  LOW,  LOW, HIGH};
		break;
	case '4':
		segData = {LOW, HIGH, HIGH,  LOW,  LOW, HIGH, HIGH};
		break;
	case '5':
	case 's':
		segData = {HIGH,  LOW, HIGH, HIGH,  LOW, HIGH, HIGH};
		break;
	case '6':
		segData = {HIGH,  LOW, HIGH, HIGH, HIGH, HIGH, HIGH};
		break;
	case '7':
	case 't':
		segData = {HIGH, HIGH, HIGH,  LOW,  LOW,  LOW,  LOW};
		break;
	case '8':
		segData = {HIGH, HIGH, HIGH, HIGH, HIGH, HIGH, HIGH};
		break;
	case '9':
	case 'g':
		segData = {HIGH, HIGH, HIGH, HIGH,  LOW, HIGH, HIGH};
		break;
	case 'e':
		segData = {HIGH,  LOW,  LOW, HIGH, HIGH, HIGH, HIGH};
		break;
	case 'c':
		segData = {HIGH,  LOW,  LOW, HIGH, HIGH, HIGH,  LOW};
		break;
	case 'v':
	case 'u':
		segData = {LOW, HIGH, HIGH, HIGH, HIGH, HIGH,  LOW};
		break;*/
	case ' ':
	default:
		uint8_t segData[7] = {LOW,LOW,LOW,LOW,LOW,LOW,LOW};
	}
	return segData[seg];
}

void VFDShield::updateNextTube()
{
	digitalWrite(A0+currentTube,LOW);					// turn off the old tube
	currentTube++;										// move to next tube
	currentTube %= 4;									// roll over if needed

	for (uint8_t pin = 2; pin < 9; pin++) 		// light needed segments
	{
		digitalWrite(pin,getSegData(digit[currentTube],pin-2]));
	}
	digitalWrite(DOT_PIN,dot[currentTube]); 	// turn dot on or off
	digitalWrite(LED_PIN,led[currentTube]);	// turn the led on of off
	digitalWrite(A0+currentTube,HIGH);					// enable the current tube
}
