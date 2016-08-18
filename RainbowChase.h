#ifndef RAINBOWCHASE_H
#define RAINBOWCHASE_H

#include "Master.h"

static int speeds[]={2,5,10,50,100};

class RainbowChase: public Master
{
	public:
	RainbowChase(){currentSpeed = 0;}
	~RainbowChase(){}
	
	virtual void loop()
	{
		FastLED.setTemperature (UncorrectedTemperature);
		uint32_t offset = (millis ()/speeds[currentSpeed]);
		for (int i = 0; i < NUM_LEDS; i++)
		{
			leds[i]=CHSV((((i*256/10)+offset)&255),255,96);
		}
		FastLED.show ();
	}
	
	virtual void modePress()
	{
		currentSpeed ++;
		if (currentSpeed >= sizeof(speeds)/sizeof(int))
		{
			currentSpeed = 0;
		}
	}
	protected:
	private:
	int currentSpeed;
};
/* RainbowChase: public Master
{
public:
RainbowChase() {currentSpeed = 0;}

~RainbowChase() {}

virtual void loop()
{
uint32_t offset= millis()/speeds[currentSpeed];
for (int i=0;i<NUM_LEDS; i++)
{
leds[i]=CHSV((((i*256/10)+offset)&255),255,96);
}
FastLED.show();
}

virtual void modePress() {
// Increment through the available speeds.
currentSpeed += 1;
if (currentSpeed >= sizeof(speeds)/sizeof(int)) {
currentSpeed = 0;
}
}

private:
int currentSpeed;
} */
#endif