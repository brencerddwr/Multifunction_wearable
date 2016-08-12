// This demo is based on the vumeter demo in the Adafruit Circuit Playground library.
#ifndef VUMETER_H
#define VUMETER_H
#include <math.h>

#include "Master.h"

#define MIC_PIN         A4  // Microphone is attached to this analog pin (A4 for circuit playground)
#define SAMPLE_WINDOW   10  // Sample window for average level
#define PEAK_HANG       48  // Time of pause before peak dot falls
#define PEAK_FALL        8  // Rate of falling peak dot
#define INPUT_FLOOR     10  // Lower range of analogRead input
//#define INPUT_CEILING  500  // Max range of analogRead input, the lower the value the more sensitive (1023 = max)

static int inputCeilings[] = { 1023,512, 256, 128 };

static byte peak = 9;        // Peak level of column; used for falling dots
static unsigned int sample;
static byte dotCount = 0;     //Frame counter for peak dot
static byte dotHangCount = 0; //Frame counter for holding peak dot
int blanking = 0;
//Used to draw a line between two points of a given color

static float fscale( float originalMin, float originalMax, float newBegin, float
newEnd, float inputValue, float curve){

	float OriginalRange = 0;
	float NewRange = 0;
	float zeroRefCurVal = 0;
	float normalizedCurVal = 0;
	float rangedValue = 0;
	boolean invFlag = 0;

	// condition curve parameter
	// limit range

	if (curve > 10) curve = 10;
	if (curve < -10) curve = -10;

	curve = (curve * -.1) ; // - invert and scale - this seems more intuitive - postive numbers give more weight to high end on output
	curve = pow(10, curve); // convert linear scale into lograthimic exponent for other pow function

	/*
	Serial.println(curve * 100, DEC);   // multply by 100 to preserve resolution
	Serial.println();
	*/

	// Check for out of range inputValues
	if (inputValue < originalMin) {
		inputValue = originalMin;
	}
	if (inputValue > originalMax) {
		inputValue = originalMax;
	}

	// Zero Refference the values
	OriginalRange = originalMax - originalMin;

	if (newEnd > newBegin){
		NewRange = newEnd - newBegin;
	}
	else
	{
		NewRange = newBegin - newEnd;
		invFlag = 1;
	}

	zeroRefCurVal = inputValue - originalMin;
	normalizedCurVal  =  zeroRefCurVal / OriginalRange;   // normalize to 0 - 1 float

	// Check for originalMin > originalMax  - the math for all other cases i.e. negative numbers seems to work out fine
	if (originalMin > originalMax ) {
		return 0;
	}

	if (invFlag == 0){
		rangedValue =  (pow(normalizedCurVal, curve) * NewRange) + newBegin;

	}
	else     // invert the ranges
	{
		rangedValue =  newBegin - (pow(normalizedCurVal, curve) * NewRange);
	}

	return rangedValue;
}


class VUMeter: public Master {
	public:
	VUMeter() { currentCeiling = 0; }
	~VUMeter() {}

	virtual void loop() {
		FastLED.setBrightness(96);
		int numPixels = NUM_LEDS;
		unsigned long startMillis= millis();  // Start of sample window
		float peakToPeak = 0;   // peak-to-peak level
		
		unsigned int signalMax = 0;
		unsigned int signalMin = 1023;
		unsigned int c, y;
		
		// collect data for length of sample window (in mS)
		while (millis() - startMillis < SAMPLE_WINDOW)
		{
			sample = analogRead(MIC_PIN);
			if (sample < 1024)  // toss out spurious readings
			{
				if (sample > signalMax)
				{
					signalMax = sample;  // save just the max levels
				}
				else if (sample < signalMin)
				{
					signalMin = sample;  // save just the min levels
				}
			}
		}
		peakToPeak = signalMax - signalMin;  // max - min = peak-peak amplitude
		
		// Serial.println(peakToPeak);
		
		
		//Scale the input logarithmically instead of linearly
		c = fscale(INPUT_FLOOR, inputCeilings[currentCeiling], 0, numPixels-1, peakToPeak, 2);
		for (int i=0;i<=c;i++){
			leds[i]=CHSV(map(i,0,numPixels-1,96,0),255,96);
		}
		
		// Turn off pixels that are below volume threshold.
		if(c > peak) {
			peak = c;        // Keep dot on top
			blanking = 0;
			dotHangCount = 0;    // make the dot hang before falling
		}
		if (c <= numPixels-1) { // Fill partial column with off pixels
			for (int blank= c; blank < numPixels;blank++)
			{
				leds[blank]=CRGB::Black;
				
			}
		}
		
		// Set the peak dot to match the rainbow gradient
		Serial.print("Peak: ");Serial.println(peak);
		//leds[peak]=CHSV(map(peak,0,numPixels-1,96,0),255,96);
		leds[peak]=CHSV(160,255,96);
		if (blanking > PEAK_FALL)
		{leds[peak]=CRGB::Black;
		}
		// Frame based peak dot animation
		if(dotHangCount > PEAK_HANG) { //Peak pause length
			if(++dotCount >= PEAK_FALL) { //Fall rate
				
				if (peak <= 0)
				{peak = 0;
					blanking++;
				}
				else
				{
					peak--;
				}
				dotCount = 0;
			}
		}
		else {
			dotHangCount++;
		}
		FastLED.show();
	}

	virtual void modePress() {
		// Ratchet through different input ceiling values.
		currentCeiling += 1;
		if (currentCeiling >= (sizeof(inputCeilings)/sizeof(int))) {
			currentCeiling = 0;
		}
		FastLED.clear();
		leds[currentCeiling]=CHSV(210,255,128);
		FastLED.show ();
		FastLED.delay(500);
	}

	private:
	int currentCeiling;
};

#endif