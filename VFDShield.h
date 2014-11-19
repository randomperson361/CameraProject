/*
 * VFDShield.h
 *
 *  Created on: Nov 18, 2014
 *      Author: Andrew
 */

/*
  Notes:
  * Macros HIGH and LOW are always passed as arguments of type byte (uint8_t).
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


#ifndef VFDSHIELD_H_
#define VFDSHIELD_H_

#define DOT_PIN				9
#define LED_PIN				10

class VFDShield
{
private:
	uint8_t currentTube;    // 0..3
	static const uint8_t segData[];

	uint8_t getSegData (uint8_t character, uint8_t seg);

public:
	VFDShield();
	virtual ~VFDShield();

	uint8_t digit[4];       // 0..9, or letter macros
	uint8_t dot[4];         // HIGH or LOW
	uint8_t led[4];			// HIGH or LOw

	void updateNextTube();
};

#endif /* VFDSHIELD_H_ */
