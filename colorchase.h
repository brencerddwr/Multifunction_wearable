#ifndef COLORCHASE_H
#define COLORCHASE_H

#include "master.h"

static uint8_t ColorChaseHue = 0;
static int ColorChaseInterframeDelay[] = {30,60,90,120};
int ColorChaseDelay = 0;
int ColorChaseLoopCounter = NUM_LEDS-1;
byte ColorChaseStep = 1;

class ColorChase: public Master
{
	public:
	ColorChase(){ColorChaseDelay = 0;}
	~ColorChase(){}
	
	virtual void loop()
	{
		FastLED.setTemperature (UncorrectedTemperature);
					
		
		leds[ColorChaseLoopCounter % NUM_LEDS] = CHSV(ColorChaseHue, 255, 255);
		if ((ColorChaseLoopCounter+2 % NUM_LEDS) <NUM_LEDS)
		{
			;
			leds[ColorChaseLoopCounter+2 % NUM_LEDS] = CHSV(ColorChaseHue+32,255,255);
		}
		if ((ColorChaseLoopCounter+4 % NUM_LEDS) <NUM_LEDS)
		{
			;
			leds[ColorChaseLoopCounter+4 % NUM_LEDS] = CHSV(ColorChaseHue+64,255,255);
		}
		if ((ColorChaseLoopCounter+6 % NUM_LEDS) <NUM_LEDS)
		{
			;
			leds[ColorChaseLoopCounter+6 % NUM_LEDS] = CHSV(ColorChaseHue+96,255,255);
		}
		if ((ColorChaseLoopCounter+8 % NUM_LEDS) <NUM_LEDS)
		{
			;
			leds[ColorChaseLoopCounter+8 % NUM_LEDS] = CHSV(ColorChaseHue+128,255,255);
		}
		
		ColorChaseStep++;
		//for(CRGB & pixel : leds(0,3)) {pixel = CHSV(ColorChaseHue,255,255);}
		
		FastLED.show();
		//fadeall();
		// Wait a little bit before we loop around and do it again
		FastLED.delay(ColorChaseInterframeDelay[ColorChaseDelay]);
		ColorChaseLoopCounter--;
		ColorChaseLoopCounter = ColorChaseLoopCounter % NUM_LEDS;
		if (ColorChaseLoopCounter<0)
		{
			ColorChaseHue= ColorChaseHue+32;
			ColorChaseLoopCounter= NUM_LEDS-1;
		}
	}

	void fadeall()
	{
		for(int i = 0;i < NUM_LEDS; i++)
		{
			leds[i].nscale8(32);
		}
	}
	
	virtual void modePress()
	{
		ColorChaseDelay ++;
		if (ColorChaseDelay >= sizeof(ColorChaseInterframeDelay)/sizeof(int))
		{
			ColorChaseDelay = 0;
		}
	}


	protected:

	private:


	
};
#endif