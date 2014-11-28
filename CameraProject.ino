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
#define DEBOUNCE_MS			20
#define NUM_MENU_ITEMS		5
#define NUM_ISO_VALUES		24
#define DEFAULT_ISO_INDEX	7
#define EC_MAX_RANGE		5.0
#define EC_STEP				(1./3.)

// Define enums to make life easier
enum menuIdentifier {ISO, ExposureCompensation, ShutterSpeed, ExposureValue, HoldOpen};

// Define const lists
uint8_t menuLabels[NUM_MENU_ITEMS][5] = {" ISO", " EC ", " SS ", " EV ", "HOLD"};
const uint16_t possibleISO[NUM_ISO_VALUES] = {20, 25, 32, 40, 50, 64, 80, 100, 125, 160, 200, 250, 320, 400, 500, 640, 800, 1000, 1250, 1600, 2000, 2500, 3200, 6400};

// Define global variables
Exposure ExpoData(LIGHT_METER_PIN,F_STOP,possibleISO[DEFAULT_ISO_INDEX]);
VFDShield Tubes;
uint8_t menuLevel, menuPos, ISOIndex, holdOpenFlag;
float ExposureComp;

void checkButtons()
{
	if(!digitalRead(SELECT_BUTTON_PIN) || !digitalRead(BACK_BUTTON_PIN) || !digitalRead(UP_BUTTON_PIN) || !digitalRead(DOWN_BUTTON_PIN))
	{
		delay(DEBOUNCE_MS);
		if(!digitalRead(SELECT_BUTTON_PIN))
		{
			menuLevel = 1;
		}
		else if (!digitalRead(BACK_BUTTON_PIN))
		{
			menuLevel = 0;
		}
		else if (!digitalRead(UP_BUTTON_PIN))
		{
			if (menuLevel == 0)
			{
				menuPos++;
				menuPos %= NUM_MENU_ITEMS;
			}
			else if (menuLevel == 1)
			{
				switch (menuPos)
				{
				case ISO:
					ISOIndex++;
					ISOIndex %= NUM_ISO_VALUES;
					break;
				case ExposureCompensation:
					if (ExposureComp < EC_MAX_RANGE-.1)
					{
						ExposureComp += EC_STEP;
					}
					break;
				case ShutterSpeed:
					// in AV mode SS values can't be changed manually, do nothing
					break;
				case ExposureValue:
					// just display Exposure value, nothing to change
					break;
				case HoldOpen:
					holdOpenFlag = 1;
					break;
				}
			}
		}
		else if (!digitalRead(DOWN_BUTTON_PIN))
		{
			if (menuLevel == 0)
			{
				if (menuPos == 0)
				{
					menuPos = NUM_MENU_ITEMS-1;
				}
				else
				{
					menuPos--;
				}
			}
			else if (menuLevel == 1)
			{
				switch (menuPos)
				{
				case ISO:
					if (ISOIndex > 0)
					{
						ISOIndex--;
					}
					break;
				case ExposureCompensation:
					if (ExposureComp > -EC_MAX_RANGE-.1)
					{
						ExposureComp -= EC_STEP;
					}
					break;
				case ShutterSpeed:
					// in AV mode SS values can't be changed manually, do nothing
					break;
				case ExposureValue:
					// just display Exposure value, nothing to change
					break;
				case HoldOpen:
					holdOpenFlag = 0;
					break;
				}
			}
		}
		updateDisplay();
	}
}

void updateDisplay()
{
	if (menuLevel == 0)			// top level of menu, display item names
	{
		Tubes.display(menuLabels[menuPos]);
	}
	else if (menuLevel == 1)	// bottom level of menu, display item values
	{
		switch (menuPos)
		{
		case ISO:
			Tubes.display(possibleISO[ISOIndex]);
			break;
		case ExposureCompensation:
			Tubes.display(ExposureComp,2);
			break;
		case ShutterSpeed:

			break;
		case ExposureValue:

			break;
		case HoldOpen:
			if (holdOpenFlag == 0)
			{

			}
			else if (holdOpenFlag == 1)
			{

			}
			break;
		}
	}
}

void updateTubes()
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
	menuLevel = 0;
	menuPos = 0;
	ISOIndex = DEFAULT_ISO_INDEX;
	ExposureComp = 0;
	holdOpenFlag = 0;
	Timer1.initialize(5000);		// 5000us = 5ms between each run, this delay can't be less than 2ms
	Timer1.attachInterrupt(updateTubes);
}

void loop()
{
	checkButtons();
	ExpoData.updateExposure();
}
