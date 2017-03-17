// concepts and some code copied from Adafruit Circuit Playground mega_demo

// include libraries
#include <FastLED.h>
#include <Adafruit_Circuit_Playground.h>
#include <Wire.h>
#include <SPI.h>
#include <Adafruit_SleepyDog.h>

// configure debug code
// uncomment the line below to enable serial debugging output and comment the second line
//bool rgbdebug=true;
bool rgbdebug=false;

// FastLED setup
#define NUM_LEDS 10
#define DATA_PIN 17
// CRGB leds[NUM_LEDS];
CRGBArray<NUM_LEDS> leds;


// include modules
#include "Master.h"
#include "Confetti.h"
#include "VUMeter.h"
#include "demo_reel_100.h"
#include "CapTouch.h"
#include "RainbowChase.h"
#include "Larson.h"
//#include "colorchase.h"

// Create an instance of each demo class.
Confetti confetti;
VUMeter vuMeter;
DemoReel100 reel;
Larson larson;
CapTouch capTouch;
RainbowChase rainbowChase;
//ColorChase colorChase;

// Make a list of all demo class instances and keep track of the currently selected one.
int currentModule = 0;
Master* modules[] = {
	&confetti,
	&rainbowChase,
	&vuMeter,
	&reel,
	&larson,
	&capTouch,
	//&colorChase
};


void setup()
{

	if (rgbdebug)
	{
		Serial.begin(115200);
		while (!Serial);
		Serial.println("Multifunction wearable debug output");
		
	}
	CircuitPlayground.begin();


	// sanity check delay - allows reprogramming if accidentally blowing power w/leds
	delay(3000);
	FastLED.addLeds<WS2812B, DATA_PIN, GRB>(leds, NUM_LEDS).setCorrection(0xB0FFFF);

	// set master brightness to enable FastLED dithering
	FastLED.setBrightness(96);
	last_start = millis();


}

void loop()
{

	// Check if slide switch is on the left (false) and go to sleep.
	while (!CircuitPlayground.slideSwitch()) {
		// Turn off the pixels, then go into deep sleep for a second.
		//CircuitPlayground.clearPixels();
		FastLED.clear();
		FastLED.show();
		Watchdog.sleep(1000);
	}

	// Check for any button presses by checking their state twice with
	// a delay inbetween.  If the first press state is different from the
	// second press state then something was pressed/released!
	bool leftFirst = CircuitPlayground.leftButton();
	bool rightFirst = CircuitPlayground.rightButton();
	FastLED.delay(10);

	// system wide dynamic brightness adjustment for all modules that use FastLED
	int mappedSensor = map (CircuitPlayground.lightSensor(), 0, 1023, 0, 255);
	int constrainedSensor = constrain(mappedSensor, 10, 255);
	FastLED.setBrightness(constrainedSensor);


	// Run current demo's main loop.
	modules[currentModule]->loop();

	// Now check for buttons that were released.
	bool leftSecond = CircuitPlayground.leftButton();
	bool rightSecond = CircuitPlayground.rightButton();

	// Left button will change the current demo.
	if (leftFirst && !leftSecond) {
		// Turn off all the pixels when entering new mode.
		//CircuitPlayground.clearPixels();
		FastLED.setTemperature (UncorrectedTemperature);
		FastLED.clear();
		FastLED.show();
		// Increment the current demo (looping back to zero if at end).
		currentModule += 1;
		if (currentModule >= (sizeof(modules)/sizeof(Master*))) {
			currentModule = 0;
		}
		if (rgbdebug)
		{
			Serial.print("Changed to module: "); Serial.println(currentModule, DEC);
		}
	}

	// Right button will change the mode of the current demo.
	if (rightFirst && !rightSecond) {
		modules[currentModule]->modePress();
	}
}
