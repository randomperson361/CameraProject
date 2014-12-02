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
#define DEBOUNCE_MS			100
#define EXP_UPDATE_DELAY	100
#define NUM_MENU_ITEMS		6
#define NUM_ISO_VALUES		24
#define NUM_EC_VALUES		31
#define NUM_DLY_VALUES		10
#define DEFAULT_ISO_INDEX	7
#define DEFAULT_EC_INDEX	15
#define DEFAULT_DLY_INDEX	0
#define EC_MAX_RANGE		5.0
#define EC_STEP				(1./3.)

// Define enums to make life easier
enum menuIdentifier {ISO, ExposureCompensation, ShutterSpeed, ExposureValue, HoldOpen, ShutterDelay};
enum dispJustification {RIGHT, LEFT};

// Define const lists
const char menuLabels[NUM_MENU_ITEMS][5] = {" ISO", " EC ", " SS ", " EV ", "HOLD", "DLAY"};
const uint16_t possibleISO[NUM_ISO_VALUES] = {20, 25, 32, 40, 50, 64, 80, 100, 125, 160, 200, 250, 320, 400, 500, 640, 800, 1000, 1250, 1600, 2000, 2500, 3200, 6400};
const float possibleEC[NUM_EC_VALUES]= {-5.00,-4.67,-4.33,-4.00,-3.67,-3.33,-3.00,-2.67,-2.33,-2.00,-1.67,-1.33,-1.00,-0.67,-0.33,0.00,0.33,0.67,1.00,1.33,1.67,2.00,2.33,2.67,3.00,3.33,3.67,4.00,4.33,4.67,5.00};
const float possibleDly[NUM_DLY_VALUES] = {0.0, 0.25, 0.5, 1.0, 2.0, 3.0, 5.0, 10.0, 20.0, 30.0};
unsigned long lastExpUpdate;

// Define global variables
Exposure ExpoData(LIGHT_METER_PIN,F_STOP,possibleISO[DEFAULT_ISO_INDEX]);
VFDShield Tubes;
uint8_t menuLevel, menuPos, ISOIndex, ECIndex, DlyIndex, shutterOpenFlag;

void openShutter()
{
	shutterOpenFlag = 1;
	digitalWrite(SHUTTER_CONTROL_PIN,HIGH);
}

void closeShutter()
{
	shutterOpenFlag = 0;
	digitalWrite(SHUTTER_CONTROL_PIN,LOW);
}

void takePhoto (uint32_t SS)
{
	noInterrupts();
	delay(1000*possibleDly[DlyIndex]);
	Tubes.clrAll();
	openShutter();
	delay(SS-17);
	closeShutter();
	interrupts();
}

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
					if (ISOIndex < NUM_ISO_VALUES-1)
					{
						ISOIndex++;
					}
					break;
				case ExposureCompensation:
					if (ECIndex < NUM_EC_VALUES-1)
					{
						ECIndex++;
					}
					break;
				case ShutterSpeed:
					// in AV mode SS values can't be changed manually, do nothing
					break;
				case ExposureValue:
					// just display Exposure value, nothing to change
					break;
				case HoldOpen:
					openShutter();
					break;
				case ShutterDelay:
					if (DlyIndex < NUM_DLY_VALUES-1)
					{
						DlyIndex++;
					}
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
					if (ECIndex > 0)
					{
						ECIndex--;
					}
					break;
				case ShutterSpeed:
					// in AV mode SS values can't be changed manually, do nothing
					break;
				case ExposureValue:
					// just display Exposure value, nothing to change
					break;
				case HoldOpen:
					closeShutter();
					break;
				case ShutterDelay:
					if (DlyIndex > 0)
					{
						DlyIndex--;
					}
					break;
				}
			}
		}
		ExpoData.ISO = possibleISO[ISOIndex];
		ExpoData.EC = possibleEC[ECIndex];
	}
	if (!digitalRead(SHUTTER_CONTROL_PIN))
	{
		takePhoto((uint32_t)ExpoData.SS*1000);
		delay(1000);
	}
}

void updateDisplay()
{
	if (menuLevel == 0)			// top level of menu, display item names
	{
		Tubes.display(menuLabels[menuPos],4);
	}
	else if (menuLevel == 1)	// bottom level of menu, display item values
	{
		switch (menuPos)
		{
		case ISO:
			Tubes.display(possibleISO[ISOIndex]);
			break;
		case ExposureCompensation:
			Tubes.display(possibleEC[ECIndex],2);
			break;
		case ShutterSpeed:
			int rawSS;
			rawSS = ExpoData.rawDispSS;
			float flSS;
			flSS = abs(rawSS)/10.0;
			if (abs(rawSS)%10 != 0)			// number has a decimal
			{
				Tubes.display(flSS,1);
			}
			else
			{
				Tubes.display(flSS,0);
			}
			if (rawSS>=0)					// number is not a reciprocal
			{
				Tubes.character[0] = '>';
			}
			else
			{
				Tubes.character[0] = '<';
			}
			break;
		case ExposureValue:
			Tubes.display(ExpoData.EV100,2);
			break;
		case HoldOpen:
			if (shutterOpenFlag == 0)
			{
				Tubes.display("CLSD",4);
			}
			else if (shutterOpenFlag == 1)
			{
				Tubes.display("OPEN",4);
			}
			break;
		case ShutterDelay:
			Tubes.display(possibleDly[DlyIndex],2);
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
	  analogReference(EXTERNAL);
	  pinMode(LIGHT_METER_PIN,INPUT);
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
	  pinMode(SELECT_BUTTON_PIN,INPUT_PULLUP);
	  pinMode(BACK_BUTTON_PIN,INPUT_PULLUP);
	  pinMode(UP_BUTTON_PIN,INPUT_PULLUP);
	  pinMode(DOWN_BUTTON_PIN,INPUT_PULLUP);
	  pinMode(SHUTTER_BUTTON_PIN,INPUT_PULLUP);
	  pinMode(SHUTTER_CONTROL_PIN,OUTPUT);
	  closeShutter();
}

void setup()
{
	initiatePins();
	menuLevel = 0;
	menuPos = 0;
	ISOIndex = DEFAULT_ISO_INDEX;
	ECIndex = DEFAULT_EC_INDEX;
	DlyIndex = DEFAULT_DLY_INDEX;
	lastExpUpdate = 0;
	Tubes.LEDs(1);
	Timer1.initialize(5000);		// 5000us = 5ms between each run, this delay can't be less than 2ms
	Timer1.attachInterrupt(updateTubes);
}

void loop()
{
	checkButtons();
	if ((millis() - lastExpUpdate) > EXP_UPDATE_DELAY)
	{
		ExpoData.updateExposure();
		updateDisplay();
		lastExpUpdate = millis();
	}
}
