#include <Arduino.h>

// Define needed constants
#define LIGHT_METER_PIN		A4
#define SELECT_BUTTON_PIN	11
#define BACK_BUTTON_PIN		A5
#define UP_BUTTON_PIN		12
#define DOWN_BUTTON_PIN		13
#define SHUTTER_BUTTON_PIN	0
#define F_STOP				11.375			// f/ of camera 182mm FL / 16mm aperture
#define SS_ARRAY_SIZE		45

// Define global variables
uint16_t ISO;
uint8_t EC;
uint16_t possibleISO[24] = {20, 25, 32, 40, 50, 64, 80, 100, 125, 160, 200, 250, 320, 400, 500, 640, 800, 1000, 1250, 1600, 2000, 2500, 3200, 6400};
float possibleSS[SS_ARRAY_SIZE] = {0.001, 0.00125, 0.0015625, 0.002, 0.0025, 0.003125, 0.004, 0.005, 0.00625, 0.008, 0.01, 0.0125, 0.01666667, 0.02, 0.025, 0.0333333, 0.04, 0.05, 0.0769231, 0.1, 0.125, 0.1666667, 0.2, 0.25, 0.3333333, 0.4, 0.5, 0.625, 0.769231, 1, 1.3, 1.6, 2, 2.5, 3, 4, 5, 6, 8, 10, 13, 15, 20, 25, 30};
int displaySS[SS_ARRAY_SIZE] = {-10000, -8000, -6400, -5000, -4000, -3200, -2500, -2000, -1600, -1250, -1000, -800, -600, -500, -400, -300, -250, -200, -130, -100, -80, -60, -50, -40, -30, -25, -20, -16, -13, 10, 13, 16, 20, 25, 30, 40, 50, 60, 80, 100, 130, 150, 200, 250, 300};  // negative numbers indicate reciprocals, divide numbers by 10 to display actual value

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

void setup()
{
	  analogReference(EXTERNAL);		// for light sensor, wire 3.3v to AREF, this must be called so that 3.3v is not shorted to 5v
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
	  pinMode(LIGHT_METER_PIN,INPUT);
	  pinMode(SELECT_BUTTON_PIN,INPUT);
	  pinMode(BACK_BUTTON_PIN,INPUT);
	  pinMode(UP_BUTTON_PIN,INPUT);
	  pinMode(DOWN_BUTTON_PIN,INPUT);
	  pinMode(SHUTTER_BUTTON_PIN,INPUT);
}

void loop()
{
	checkButtons();
}

/*

  This program demonstrates the capabilities of the Axiris IV-3 Arduino Shield.

  Notes:
  * Macros HIGH and LOW are always passed as arguments of type byte (uint8_t).
  * Pins 2..9 are connected to the segments and dots of the tubes.
  * Pins A0..A3 are connected to the grids of the tubes.
*/


/*
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
/*
static  byte  digit_seg_data[10*7] PROGMEM =
{
// pin    2     3     4     5     6     7     8
        HIGH, HIGH, HIGH, HIGH, HIGH, HIGH,  LOW,
         LOW, HIGH, HIGH,  LOW,  LOW,  LOW,  LOW,
        HIGH, HIGH,  LOW, HIGH, HIGH,  LOW, HIGH,
        HIGH, HIGH, HIGH, HIGH,  LOW,  LOW, HIGH,
         LOW, HIGH, HIGH,  LOW,  LOW, HIGH, HIGH,
        HIGH,  LOW, HIGH, HIGH,  LOW, HIGH, HIGH,
        HIGH,  LOW, HIGH, HIGH, HIGH, HIGH, HIGH,
        HIGH, HIGH, HIGH,  LOW,  LOW,  LOW,  LOW,
        HIGH, HIGH, HIGH, HIGH, HIGH, HIGH, HIGH,
        HIGH, HIGH, HIGH, HIGH,  LOW, HIGH, HIGH
};


typedef  struct  _TUBE
{
  byte    digit;      // 0..9
  byte    dot;        // HIGH or LOW
}
  TUBE;


typedef  struct  _LED
{
  byte    pwm_off;    // 0..7, 8 means always on, 255 means repeat from start
  byte    wait_cnt;   // 0..65535
}
  LED;


// Display state of the tubes (read-write)

static  TUBE    tube_list[4] =
{
  { 1,  LOW },
  { 2,  LOW },
  { 3,  LOW },
  { 4, HIGH }
};


// LED PWM sequence data (read-only)

static  LED  led_seq[] PROGMEM =
{
  {   8, 200 },
  {   7,   4 },
  {   6,   4 },
  {   5,   3 },
  {   4,   3 },
  {   3,   2 },
  {   2,   3 },
  {   1,   3 },
  {   0,   4 },
  {   1,   3 },
  {   2,   3 },
  {   3,   2 },
  {   4,   3 },
  {   5,   3 },
  {   6,   4 },
  {   7,   4 },
  { 255,   0 }
};


static  byte  cur_tube      = 3;    // 0..3
static  word  tube_wait_cnt = 0;
static  word  dot_wait_cnt  = 83;
static  byte  led_pwm_step  = 0;    // 0..31
static  byte  led_pwm_off   = 0;
static  byte  led_seq_index = 0;
static  byte  led_wait_cnt  = 0;

static
void  step_led_seq (void)
{
  byte  pwm_off;

  // Fetch the PWM off position. If 255, start over from the beginning of the sequence data.
  for (;;)
  {
    pwm_off = pgm_read_byte(&led_seq[led_seq_index].pwm_off);
    if (pwm_off != 255) break;
    led_seq_index = 0;
  }

  led_pwm_off  = pwm_off;
  led_wait_cnt = pgm_read_byte(&led_seq[led_seq_index].wait_cnt);

  led_seq_index++;
}


void setup()
{
  step_led_seq();
}


void loop()
{
  byte   *digit_seg_p;
  byte    digit;
  byte    pin;
  byte    tube;

  // Clear the current Ax pin
  digitalWrite(A0+cur_tube,LOW);

  // Next tube
  cur_tube++;
  cur_tube %= 4;

  digit = tube_list[cur_tube].digit;
  digit_seg_p = digit_seg_data + 7*digit;
  for (pin = 2; pin < 9; pin++, digit_seg_p++) digitalWrite(pin,pgm_read_byte(digit_seg_p));
  digitalWrite(pin,tube_list[cur_tube].dot);

  // Enable the current tube
  digitalWrite(A0+cur_tube,HIGH);

  tube_wait_cnt++;
  if (tube_wait_cnt == 166)
  {
    // Reset the wait counter
    tube_wait_cnt = 0;

    // Change the digits
    for (tube = 0; tube < 4; tube++)
    {
      digit = tube_list[tube].digit;
      if (digit == 9) digit = 0; else digit++;
      tube_list[tube].digit = digit;
    }
  }

  dot_wait_cnt++;
  if (dot_wait_cnt == 498)
  {
    // Reset the wait counter
    dot_wait_cnt = 0;

    // Animate the dot as an incrementing 4-bit binary number
    tube_list[3].dot ^= 1;
    if (tube_list[3].dot == LOW)
    {
      tube_list[2].dot ^= 1;
      if (tube_list[2].dot == LOW)
      {
        tube_list[1].dot ^= 1;
        if (tube_list[1].dot == LOW)
        {
          tube_list[0].dot ^= 1;
        }
      }
    }
  }


  // LED PWM

  if (led_pwm_step == led_pwm_off)
  {
    digitalWrite(10,LOW);
  }
  else
  {
    if (led_pwm_step == 0)
    {
      digitalWrite(10,HIGH);
    }
  }

  led_pwm_step++;
  if (led_pwm_step == 8)
  {
    led_pwm_step = 0;

    // LED PWM sequence
    led_wait_cnt--;
    if (led_wait_cnt == 0) step_led_seq();
  }

  // Delay 2 ms => at most 500 cycles/second
  delay(2);
}
*/
