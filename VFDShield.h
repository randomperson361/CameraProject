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
	volatile uint8_t currentTube;    // 0..3, must be volatile in case updateNextTube is called by interrupt
	static const uint8_t segData[30];

	uint8_t getSegDataIndex (uint8_t character);

public:
	VFDShield();
	virtual ~VFDShield();

	uint8_t character[4];       // 0..9, or letter macros
	uint8_t dot[4];         // HIGH or LOW
	uint8_t led[4];			// HIGH or LOw

	void display (uint8_t arr[], uint8_t size = 4);
	void display (uint16_t num);
	void display (float num, uint8_t decimals = 1);
	void updateNextTube();
	// TODO: add tube update interrupt code into this class rather than being external
};

#endif /* VFDSHIELD_H_ */
