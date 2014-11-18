/*
 * Exposure.h
 *
 *  Created on: Nov 18, 2014
 *      Author: Andrew
 */

#ifndef EXPOSURE_H_
#define EXPOSURE_H_

class Exposure
{
private:
	static const uint16_t possibleISO[24]; //{20, 25, 32, 40, 50, 64, 80, 100, 125, 160, 200, 250, 320, 400, 500, 640, 800, 1000, 1250, 1600, 2000, 2500, 3200, 6400};
	static const float possibleSS[46]; //{0.001, 0.00125, 0.0015625, 0.002, 0.0025, 0.003125, 0.004, 0.005, 0.00625, 0.008, 0.01, 0.0125, 0.01666667, 0.02, 0.025, 0.0333333, 0.04, 0.05, 0.0769231, 0.1, 0.125, 0.1666667, 0.2, 0.25, 0.3333333, 0.4, 0.5, 0.625, 0.769231, 1, 1.3, 1.6, 2, 2.5, 3, 4, 5, 6, 8, 10, 13, 15, 20, 25, 30, 60};
	static const int displaySS[46]; //{-10000, -8000, -6400, -5000, -4000, -3200, -2500, -2000, -1600, -1250, -1000, -800, -600, -500, -400, -300, -250, -200, -130, -100, -80, -60, -50, -40, -30, -25, -20, -16, -13, 10, 13, 16, 20, 25, 30, 40, 50, 60, 80, 100, 130, 150, 200, 250, 300, 600};  // negative numbers indicate reciprocals, divide numbers by 10 to display actual value
	uint8_t lightMeterPin;
	float RawToLux(int raw);
	float readLightMeter();
	float calcEV100(float lux);
	float calcEVx(float EV100);
	float calcExposureTime(float EV);
	uint8_t calcSSIndex(float exposureTime);

public:
	Exposure(int pin);
	virtual ~Exposure();

	uint16_t ISO;
	float fStop;
	uint8_t EC;
	float Expo;

	void updateExposure();
};

#endif /* EXPOSURE_H_ */
