#include <Arduino.h>

// Define pins
#define LIGHT_METER_PIN		A4
#define SELECT_BUTTON_PIN	1
#define BACK_BUTTON_PIN		A5				// TODO: determine if i need a back button
#define UP_BUTTON_PIN		11
#define DOWN_BUTTON_PIN		12
#define SHUTTER_BUTTON_PIN	0
#define SHUTTER_CONTROL_PIN 13

// Define needed constants
#define F_STOP				11.375			// f/ of camera 182mm FL / 16mm aperture
#define SS_ARRAY_SIZE		46
#define TUBE_OFF			10
#define TUBE_E				11
#define TUBE_C				12
#define TUBE_v				13
#define DOT_PIN				9
#define LED_PIN				10

// Define tube stuff
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

static const byte tubeChar[14*7] PROGMEM =
{
// pin    2     3     4     5     6     7     8
        HIGH, HIGH, HIGH, HIGH, HIGH, HIGH,  LOW,	// 0
         LOW, HIGH, HIGH,  LOW,  LOW,  LOW,  LOW,	// 1
        HIGH, HIGH,  LOW, HIGH, HIGH,  LOW, HIGH,	// 2
        HIGH, HIGH, HIGH, HIGH,  LOW,  LOW, HIGH,	// 3
         LOW, HIGH, HIGH,  LOW,  LOW, HIGH, HIGH,	// 4
        HIGH,  LOW, HIGH, HIGH,  LOW, HIGH, HIGH,	// 5
        HIGH,  LOW, HIGH, HIGH, HIGH, HIGH, HIGH,	// 6
        HIGH, HIGH, HIGH,  LOW,  LOW,  LOW,  LOW,	// 7
        HIGH, HIGH, HIGH, HIGH, HIGH, HIGH, HIGH,	// 8
        HIGH, HIGH, HIGH, HIGH,  LOW, HIGH, HIGH,	// 9
         LOW,  LOW,  LOW,  LOW,  LOW,  LOW,  LOW,  	// TUBE_OFF
        HIGH,  LOW,  LOW, HIGH, HIGH, HIGH, HIGH,	// TUBE_E
        HIGH,  LOW,  LOW, HIGH, HIGH, HIGH,  LOW,	// TUBE_C
         LOW, HIGH, HIGH, HIGH, HIGH, HIGH,  LOW	// TUBE_V
};

typedef  struct  _TUBE
{
	byte    digit;      // 0..9, or letter macros
	byte    dot;        // HIGH or LOW
	byte	led;		// HIGH or LOw
}
  	TUBE;

static TUBE tubeData[4] =
{
	{ TUBE_OFF, LOW, LOW },
	{ TUBE_OFF, LOW, LOW },
	{ TUBE_OFF, LOW, LOW },
	{ TUBE_OFF, LOW, LOW }
};

// Define global variables
static byte currentTube = 3;    // 0..3
static uint16_t ISO;
static uint8_t EC;
static const uint16_t possibleISO[24] = {20, 25, 32, 40, 50, 64, 80, 100, 125, 160, 200, 250, 320, 400, 500, 640, 800, 1000, 1250, 1600, 2000, 2500, 3200, 6400};
static const float possibleSS[SS_ARRAY_SIZE] = {0.001, 0.00125, 0.0015625, 0.002, 0.0025, 0.003125, 0.004, 0.005, 0.00625, 0.008, 0.01, 0.0125, 0.01666667, 0.02, 0.025, 0.0333333, 0.04, 0.05, 0.0769231, 0.1, 0.125, 0.1666667, 0.2, 0.25, 0.3333333, 0.4, 0.5, 0.625, 0.769231, 1, 1.3, 1.6, 2, 2.5, 3, 4, 5, 6, 8, 10, 13, 15, 20, 25, 30, 60};
static const int displaySS[SS_ARRAY_SIZE] = {-10000, -8000, -6400, -5000, -4000, -3200, -2500, -2000, -1600, -1250, -1000, -800, -600, -500, -400, -300, -250, -200, -130, -100, -80, -60, -50, -40, -30, -25, -20, -16, -13, 10, 13, 16, 20, 25, 30, 40, 50, 60, 80, 100, 130, 150, 200, 250, 300, 600};  // negative numbers indicate reciprocals, divide numbers by 10 to display actual value

// Define program functions
float RawToLux(int raw)
{
	float logLux = raw * 5.0 / 1024;
	return pow(10, logLux);
}

float readLightMeter()					// set up for GA1A1S202WP, buy from adafruit
{
	int rawValue = analogRead(LIGHT_METER_PIN);
	return RawToLux(rawValue);
}

float calcEV100(float lux)				// calculates the EV at ISO 100 when given a lux value
{
	return 1.4427*log(lux)-1.3219;
}

float calcEVx(float EV100)					// adjusts EV100 for correct ISO speed and exposure compensation
{
	return EV100 + (log(ISO/100))/(log(2)) - EC;
}

float calcExposureTime(float EV)
{
	return pow(F_STOP,2)/pow(2,EV);
}

uint8_t calcSSIndex(float exposureTime)
{
	int i = 0;
	while (exposureTime>possibleSS[i])
	{
		i++;
		if (i==SS_ARRAY_SIZE-1)
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

void checkButtons()
{

}

void updateNextTube()
{
	byte   *digit_seg_p;
	byte    digit;
	byte    pin;

	digitalWrite(A0+currentTube,LOW);					// turn off the old tube
	currentTube++;										// move to next tube
	currentTube %= 4;									// roll over if needed

	digit = tubeData[currentTube].digit;
	digit_seg_p = tubeChar + 7*digit;					// TODO: i think this line got fucked up
	for (pin = 2; pin < 9; pin++, digit_seg_p++) 		// light needed segments
	{
		digitalWrite(pin,pgm_read_byte(digit_seg_p));
	}
	digitalWrite(DOT_PIN,tubeData[currentTube].dot); 	// turn dot on or off
	digitalWrite(LED_PIN,tubeData[currentTube].led);	// turn the led on of off
	digitalWrite(A0+currentTube,HIGH);					// enable the current tube
}

void serialShowTubes()
{

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
	analogReference(EXTERNAL);		// for light sensor, wire 3.3v to AREF, this must be called so that 3.3v is not shorted to 5v
	initiatePins();
	Serial.begin(9600);
}

void loop()
{
	checkButtons();

	serialShowTubes();	// enable this for debugging
	//updateNextTube();	 // enable this when tubes are actuall connected
	delay(2);		// Delay 2 ms => at most 500 cycles/second
}

// testing code
/*
 * 	for (long l = 5; l<=50000; l*=2)
	{
		Serial.print("Lux: ");
		Serial.println(l);
		Serial.print("EV100: ");
		float EV100 = calcEV100((float)l);
		Serial.println(EV100);
		Serial.print("EVx: ");
		float EVx = calcEVx(EV100);
		Serial.println(EVx);
		Serial.print("ISO: ");
		Serial.println(ISO);
		Serial.print("Expo: ");
		float Expo = calcExposureTime(EVx);
		Serial.println(Expo,7);
		Serial.print("SS: ");
		float SS = possibleSS[calcSSIndex(Expo)];
		Serial.println(SS,7);
		Serial.print("Disp SS: ");
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

		Serial.println("\n\n");

		while(Serial.read()==-1);
	}
*/
