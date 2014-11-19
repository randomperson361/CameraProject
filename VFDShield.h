/*
 * VFDShield.h
 *
 *  Created on: Nov 18, 2014
 *      Author: Andrew
 */

#ifndef VFDSHIELD_H_
#define VFDSHIELD_H_

#define DOT_PIN				9
#define LED_PIN				10

class VFDShield
{
private:
	uint8_t currentTube;    // 0..3
	static const uint8_t segData[30];

	uint8_t getSegDataIndex (uint8_t character);

public:
	VFDShield();
	virtual ~VFDShield();

	uint8_t character[4];       // 0..9, or letter macros
	uint8_t dot[4];         // HIGH or LOW
	uint8_t led[4];			// HIGH or LOw

	void updateNextTube();
};

#endif /* VFDSHIELD_H_ */
