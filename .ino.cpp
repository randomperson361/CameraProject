//This is a automatic generated file
//Please do not modify this file
//If you touch this file your change will be overwritten during the next build
//This file has been generated on 2014-11-18 11:52:56

#include "Arduino.h"
#include <Arduino.h>
float RawToLux(int raw) ;
float readLightMeter()					 ;
float calcEV100(float lux)				 ;
float calcEVx(float EV100)					 ;
float calcExposureTime(float EV) ;
uint8_t calcSSIndex(float exposureTime) ;
void checkButtons() ;
void updateNextTube() ;
void initiatePins() ;
void setup() ;
void loop() ;


#include "CameraProject.ino"
