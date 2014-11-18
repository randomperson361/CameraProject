/*
 * Exposure.cpp
 *
 *  Created on: Nov 18, 2014
 *      Author: Andrew
 */

#include "Arduino.h"
#include "Exposure.h"

Exposure::Exposure(int pin)
{
	lightMeterPin = pin;
	ISO = 100;
	EC = 0;
	Expo = 0;
	fStop = 11.375;
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
		if (i==46-1)
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
	uint16_t l = readLightMeter();
	float EV100 = calcEV100((float)l);
	float EVx = calcEVx(EV100);
	Expo = calcExposureTime(EVx);
	float SS = possibleSS[calcSSIndex(Expo)];
	int rawDispSS = displaySS[calcSSIndex(Expo)];
	if (rawDispSS < 0)
	{
		Serial.print("1/");
		Serial.println(-rawDispSS/10);
	}
	else
	{
		Serial.println(rawDispSS/10);
	}
}
