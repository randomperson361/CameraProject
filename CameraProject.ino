#include <Arduino.h>

// Define needed constants
#define LIGHT_METER_PIN		A4
#define SELECT_BUTTPN_PIN	11
#define BACK_BUTTON_PIN		A5
#define UP_BUTTON_PIN		12
#define DOWN_BUTTON_PIN		13

// Define global variables
uint16_t ISO;
uint8_t EC;


// Define program functions
float readLightMeter()
{
	float lux;


	return lux;
}

float calcEV100()
{
	float EV100;
	return EV100;
}

float calcShutterSpeed()
{

}

void setup()
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
	  pinMode(LIGHT_METER_PIN,INPUT);
}

void loop()
{

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
