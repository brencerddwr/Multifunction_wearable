#ifndef DEMOREEL100_H
#define DEMOREEL100_H
#include "Master.h"

#define BRIGHTNESS          96
#define FRAMES_PER_SECOND  300



uint8_t gCurrentPatternNumber = 0; // Index number of which pattern is current
uint8_t gHue = 0; // rotating "base color" used by many of the patterns



class DemoReel100: public Master
{

	public:
	DemoReel100(){};
	~DemoReel100(){};

	virtual void loop()
	{
		FastLED.setTemperature (UncorrectedTemperature);
		// Call the current pattern function once, updating the 'leds' array
		switch (gCurrentPatternNumber)
		{
			//case 0:
			//rainbow ();
			//break;
			case 0:
			rainbowWithGlitter();
			break;
			case 1:
			confettid();
			break;
		}

		// send the 'leds' array out to the actual LED strip
		FastLED.show();
		// insert a delay to keep the framerate modest
		FastLED.delay(1000/FRAMES_PER_SECOND);

		// do some periodic updates
		EVERY_N_MILLISECONDS( 20 ) { gHue++; } // slowly cycle the "base color" through the rainbow

		
	}
	virtual void modePress()
	{
		gCurrentPatternNumber = (gCurrentPatternNumber + 1) % 2;
	}
	
	void rainbow()
	{
		// FastLED's built-in rainbow generator
		fill_rainbow( leds, NUM_LEDS, gHue, 7);
	}

	void rainbowWithGlitter()
	{
		// built-in FastLED rainbow, plus some random sparkly glitter
		rainbow();
		addGlitter(80);
	}

	void addGlitter( fract8 chanceOfGlitter)
	{
		if( random8() < chanceOfGlitter) {
			leds[ random16(NUM_LEDS) ] += CRGB::White;
		}
	}

	void confettid()
	{
		// random colored speckles that blink in and fade smoothly
		fadeToBlackBy( leds, NUM_LEDS, 10);
		int pos = random16(NUM_LEDS);
		leds[pos] += CHSV( gHue + random8(64), 200, 255);
	}

};

#endif