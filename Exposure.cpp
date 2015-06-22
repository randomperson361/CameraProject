/*
 * Exposure.cpp
 *
 *  Created on: Nov 18, 2014
 *      Author: Andrew
 */

#include "Arduino.h"
#include "Exposure.h"

const float Exposure::possibleSS[NUM_SS_VALUES] = {0.01666667, 0.02, 0.025, 0.0333333, 0.04, 0.05, 0.0769231, 0.1, 0.125, 0.1666667, 0.2, 0.25, 0.3333333, 0.4, 0.5, 0.625, 0.769231, 1, 1.3, 1.6, 2, 2.5, 3, 4, 5, 6, 8, 10, 13, 15, 20, 25, 30, 60, 90, 120, 150, 180, 210, 240, 270, 300};
const int Exposure::displaySS[NUM_SS_VALUES] = {-600, -500, -400, -300, -250, -200, -130, -100, -80, -60, -50, -40, -30, -25, -20, -16, -13, 10, 13, 16, 20, 25, 30, 40, 50, 60, 80, 100, 130, 150, 200, 250, 300, 600, 900, 1200, 1500, 1800, 2100, 2400, 2700, 3000};  // negative numbers indicate reciprocals, divide numbers by 10 to display actual value

Exposure::Exposure(int pin, float fstop, uint16_t iso, float ec)
{
	lightMeterPin = pin;
	ISO = iso;
	EC = ec;
	fStop = fstop;
	updateExposure();
}

Exposure::~Exposure()
{
	// TODO Auto-generated destructor stub
}

float Exposure::RawToLux(int raw)
{
	float logLux = raw * 5.0 / 1024;
	return pow(10, logLux);
}

float Exposure::readLightMeter()					// set up for GA1A1S202WP, buy from adafruit
{
	int rawValue = analogRead(lightMeterPin);
	return RawToLux(rawValue);
}

float Exposure::calcEV100(float lux)				// calculates the EV at ISO 100 when given a lux value
{
	return 1.4427*log(lux)-1.3219;
}

float Exposure::calcEVx(float EV100)					// adjusts EV100 for correct ISO speed and exposure compensation
{
	return EV100 + (log(ISO/100))/(log(2)) - EC;
}

float Exposure::calcExposureTime(float EV)
{
	return pow(fStop,2)/pow(2,EV);
}

uint8_t Exposure::calcSSIndex(float exposureTime)
{
	int i = 0;
	while (exposureTime>possibleSS[i])
	{
		i++;
		if (i==NUM_SS_VALUES-1)
		{
			break;
		}
	}
	if (abs(possibleSS[i]-exposureTime) > abs(possibleSS[i-1]-exposureTime))
	{
		i--;
	}
	return i;
}

void Exposure::updateExposure()
{
	lux = readLightMeter();
	EV100 = calcEV100((float)lux);
	EVx = calcEVx(EV100);
	Expo = calcExposureTime(EVx);
	SS = possibleSS[calcSSIndex(Expo)];
	rawDispSS = displaySS[calcSSIndex(Expo)];
}
