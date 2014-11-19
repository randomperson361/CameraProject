#include <Arduino.h>
#include "Exposure.h"
#include "VFDShield.h"
#include "TimerOne.h"

// Define pins
#define LIGHT_METER_PIN		A4
#define SELECT_BUTTON_PIN	1
#define BACK_BUTTON_PIN		A5
#define UP_BUTTON_PIN		11
#define DOWN_BUTTON_PIN		12
#define SHUTTER_BUTTON_PIN	0
#define SHUTTER_CONTROL_PIN 13

// Define needed constants
#define F_STOP				11.375		// 182mm fl / 16mm objective

// Define global variables
Exposure ExpoData(LIGHT_METER_PIN,F_STOP);
VFDShield Tubes;

void tubeTest()
{
	uint8_t testStr[] = "0123456789 abcdefghijklmnopqrstuvwxyz ";
	uint8_t strLen = 38;
	for (int i = 0; i<strLen-3; i++)
	{
		Tubes.character[0] = testStr[i];
		Tubes.character[1] = testStr[i+1];
		Tubes.character[2] = testStr[i+2];
		Tubes.character[3] = testStr[i+3];
		delay(500);
	}
}

void updateDisplay()
{
	Tubes.updateNextTube();
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
	Timer1.initialize(5000);		// 5000us = 5ms between each run, this delay can't be less than 2ms
	Timer1.attachInterrupt(updateDisplay);
}

void loop()
{
	tubeTest();
	//ExpoData.updateExposure();
}
