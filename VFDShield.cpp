/*
 * VFDShield.cpp
 *
 *  Created on: Nov 18, 2014
 *      Author: Andrew
 */

#include "Arduino.h"
#include "VFDShield.h"

/*
  Notes:
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

const uint8_t VFDShield::segData[32]  =		// stores segment data for the tube in a single byte, first bit is unused, bits 1-7 correspond to pins 2-8
{
		0b01111110,			// 0, o, O
		0b00110000,			// 1, i, I
		0b01101101,			// 2, z, Z
		0b01111001,			// 3
		0b00110011,			// 4
		0b01011011,			// 5, s, S
		0b01011111,			// 6
		0b01110000,			// 7
		0b01111111,			// 8
		0b01111011,			// 9, g. G
		0b01110111,			// a, A
		0b00011111,			// b, B
		0b01001110,			// c, C
		0b00111101,			// d, D
		0b01001111,			// e, E
		0b01000111,			// f, F
		0b00110111,			// h, k, x, H. K, X
		0b00111100,			// j, J
		0b00001110,			// l. L
		0b01010100,			// m, M
		0b00010101,			// n, N
		0b01100111,			// p, P
		0b01110011,			// q, Q
		0b00000101,			// r, R
		0b00001111,			// t, T
		0b00111110,			// u, U
		0b00011100,			// v, V, <
		0b00101010,			// w, W
		0b00111011,			// y, Y
		0b00000001,			// -
		0b01100010,			// >
		0b00000000			// space , default
};

VFDShield::VFDShield()
{
	currentTube = 0;
	clrAll();
}

VFDShield::~VFDShield()
{
	// TODO Auto-generated destructor stub
}

uint8_t VFDShield::getSegDataIndex(uint8_t character)
{
	uint8_t index;
	switch (character)
	{
	case '0':
	case 'o':
	case 'O':
	case 0:
		index = 0;
		break;
	case '1':
	case 'i':
	case 'I':
	case 1:
		index = 1;
		break;
	case '2':
	case 'z':
	case 'Z':
	case 2:
		index = 2;
		break;
	case '3':
	case 3:
		index = 3;
		break;
	case '4':
	case 4:
		index = 4;
		break;
	case '5':
	case 's':
	case 'S':
	case 5:
		index = 5;
		break;
	case '6':
	case 6:
		index = 6;
		break;
	case '7':
	case 7:
		index = 7;
		break;
	case '8':
	case 8:
		index = 8;
		break;
	case '9':
	case 'g':
	case 'G':
	case 9:
		index = 9;
		break;
	case 'a':
	case 'A':
		index = 10;
		break;
	case 'b':
	case 'B':
		index = 11;
		break;
	case 'c':
	case 'C':
		index = 12;
		break;
	case 'd':
	case 'D':
		index = 13;
		break;
	case 'e':
	case 'E':
		index = 14;
		break;
	case 'f':
	case 'F':
		index = 15;
		break;
	case 'h':
	case 'k':
	case 'x':
	case 'H':
	case 'K':
	case 'X':
		index = 16;
		break;
	case 'j':
	case 'J':
		index = 17;
		break;
	case 'l':
	case 'L':
		index = 18;
		break;
	case 'm':
	case 'M':
		index = 19;
		break;
	case 'n':
	case 'N':
		index = 20;
		break;
	case 'p':
	case 'P':
		index = 21;
		break;
	case 'q':
	case 'Q':
		index = 22;
		break;
	case 'r':
	case 'R':
		index = 23;
		break;
	case 't':
	case 'T':
		index = 24;
		break;
	case 'u':
	case 'U':
		index = 25;
		break;
	case 'v':
	case 'V':
	case '<':
		index = 26;
		break;
	case 'w':
	case 'W':
		index = 27;
		break;
	case 'y':
	case 'Y':
		index = 28;
		break;
	case '-':
		index = 29;
		break;
	case '>':
		index = 30;
		break;
	case ' ':
	default:
		index = 31;
	}
	return index;
}

void VFDShield::clrNums()
{
	for (int i = 0; i<4; i++)
	{
		character[i] = ' ';
		dot[i] = LOW;
	}
}

void VFDShield::clrLEDs()
{
	LEDs(0);
}

void VFDShield::clrAll()
{
	clrNums();
	clrLEDs();
}

void VFDShield::LEDs (uint8_t on)
{
	for (int i = 0; i<4; i++)
	{
		led[i] = on;
	}
}
void VFDShield::display(const char *arr, uint8_t size)
{
	clrNums();
	if (size <= 4)
	{
		for (int i=0; i<4-size; i++)
		{
			character[i] = ' ';
		}
		for (int j=4-size; j<size; j++)
		{
			character[j] = arr[j];
		}
	}
	else if (size >= 4)
	{
		for (int i=0; i<4; i++)
		{
			character[i] = arr[i];
		}
	}
	// TODO: add scrolling feature for strings longer than 4 characters
}

void VFDShield::display(const uint16_t num, uint8_t just)
{
	clrNums();
	// TODO: this could be written more efficiently as a loop
	uint8_t tenthousands, thousands, hundreds, tens, ones;
	tenthousands = num/10000;
	thousands = (num-(tenthousands*10000))/1000;					// tenthousands are discarded
	hundreds = (num-(tenthousands*10000)-(thousands*1000))/100;
	tens = (num-(tenthousands*10000)-(thousands*1000)-(hundreds*100))/10;
	ones = (num-(tenthousands*10000)-(thousands*1000)-(hundreds*100)-(tens*10));
	if (just == 0)
	{
		character[0] = thousands;
		character[1] = hundreds;
		character[2] = tens;
		character[3] = ones;
		if (num<1000)
		{
			character[0] = ' ';
			if (num<100)
			{
				character[1] = ' ';
				if (num<10)
					character[2] = ' ';
			}
		}
	}
	else
	{
		if (num>=1000)
		{
			character[0] = thousands;
			character[1] = hundreds;
			character[2] = tens;
			character[3] = ones;
		}
		else if (num>=100)
		{
			character[0] = hundreds;
			character[1] = tens;
			character[2] = ones;
		}
		else if (num>=10)
		{
			character[0] = tens;
			character[1] = ones;
		}
		else
		{
			character[0] = ones;
		}
	}
}

void VFDShield::display(const float num, uint8_t decimals)
{
	float dispNum = num;
	clrNums();
	if (decimals > 3)
	{
		decimals = 3;
	}
	if (decimals > 2 && num < 0)
	{
		decimals = 2;
	}
	// these two functions if statements round the number to the third decimal place to fix floating point truncation
	if (num > 0)
	{
		dispNum += 0.0005;
	}
	else if (num < 0)
	{
		dispNum -= 0.0005;
	}
	uint16_t integerNum = abs(dispNum * pow(10,decimals));
	display(integerNum);
	if (num < 0)
	{
		character[0] = '-';
	}
	if (decimals > 0)
	{
		dot[3-decimals] = HIGH;
	}
	// TODO add scrolling to display bigger numbers
}

void VFDShield::updateNextTube()
{
	digitalWrite(A0+currentTube,LOW);					// turn off the old tube
	currentTube++;										// move to next tube
	currentTube %= 4;									// roll over if needed
	for (uint8_t pin = 2; pin < 9; pin++) 				// light needed segments
	{
		digitalWrite(pin,((segData[getSegDataIndex(character[currentTube])]>>(8-pin))&1));		// write correct segments for character
	}
	digitalWrite(DOT_PIN,dot[currentTube]); 			// turn dot on or off
	digitalWrite(LED_PIN,led[currentTube]);				// turn the led on of off
	digitalWrite(A0+currentTube,HIGH);					// enable the current tube
}
