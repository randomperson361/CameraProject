/*
 * Exposure.h
 *
 *  Created on: Nov 18, 2014
 *      Author: Andrew
 */

#ifndef EXPOSURE_H_
#define EXPOSURE_H_

#define NUM_SS_VALUES 42

class Exposure
{
private:
	static const float possibleSS[NUM_SS_VALUES];
	static const int displaySS[NUM_SS_VALUES];
	uint8_t lightMeterPin;
	float RawToLux(int raw);
	float readLightMeter();
	float calcEV100(float lux);
	float calcEVx(float EV100);
	float calcExposureTime(float EV);
	uint8_t calcSSIndex(float exposureTime);

public:
	Exposure(int pin, float fstop = 8, uint16_t iso = 100, uint8_t ec = 0);
	virtual ~Exposure();
	uint16_t ISO;
	float fStop;
	uint8_t EC;
	float Expo;
	float EVx;
	float EV100;
	int rawDispSS;
	uint16_t lux;
	float SS;

	void updateExposure();
};

#endif /* EXPOSURE_H_ */
