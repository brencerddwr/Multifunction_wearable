#ifndef CONFETTI_H
#define CONFETTI_H
#include "Master.h"

// Start of confetti settings

unsigned long last_start;
unsigned int new_led_delay = 5000 / NUM_LEDS;
unsigned int fade_delay = 30;
unsigned int tempLow = 70;
unsigned int tempHi = 96;
bool tempReact = true;

// dynamic color temperature code part
CRGB colorShift;
CRGB blue = CHSV(HUE_BLUE, 255, 255);
CRGB red = CHSV(HUE_RED, 255, 255);
CRGB white = CRGB::White;
CRGBPalette16 gPal;

// end of confetti settings


class Confetti: public Master
{
	public:
	Confetti(){};
	~Confetti(){};
	virtual void loop()
	{
		fadeToBlackBy( leds, NUM_LEDS, 10);
		gPal =CRGBPalette16(blue,white,red);

		if (rgbdebug)
		{
			leds[0] = CHSV(0, 0, 255);
		}
		if (millis() - last_start > new_led_delay)
		{
			int mappedSensor = map (CircuitPlayground.lightSensor(), 0, 1023, 0, 255);
			int constrainedSensor = constrain(mappedSensor, 10, 255);
			int mappedTempF = constrain(map(CircuitPlayground.temperatureF() * 100, tempLow*100, tempHi*100, 0, 240),0,240);
			

			// comment next lint to disable dynamic brightness
			FastLED.setBrightness(constrainedSensor);

			if (tempReact)
			{
				colorShift = ColorFromPalette(gPal, mappedTempF);
				FastLED.setTemperature( colorShift);
			}
			else {
				FastLED.setTemperature (UncorrectedTemperature);
			}


			if (rgbdebug)
			{
				Serial.print("Temperature ");
				if (tempReact){
					Serial.print(CircuitPlayground.temperatureF());
					Serial.print(" *F, mapped temp value: ");
					Serial.println(mappedTempF);
				}
				else {
					Serial.print(CircuitPlayground.temperatureF ());
					Serial.println (" *F");
				}
			}



			byte x = random16(0, NUM_LEDS);
			if (!leds[x])
			{
				leds[ x ] += CHSV( random8(), random8( 128, 255), 255);
				last_start = millis();
			}
		}
		FastLED.show();
		FastLED.delay(fade_delay);
	}

	virtual void modePress()
	{
		tempReact = !tempReact;
	};

};
#endif