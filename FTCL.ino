/********************************************************
FTCL
7/1/14
********************************************************/
#include <Adafruit_NeoPixel.h>
#include "WS2812_Definitions.h"

#define PIN 6
#define LED_COUNT 5

// Parameter 1 = number of pixels in leds
// Parameter 2 = pin number (most are valid)
// Parameter 3 = pixel type flags, add together as needed:
//   NEO_KHZ800  800 KHz bitstream (most NeoPixel products w/WS2812 LEDs)
//   NEO_KHZ400  400 KHz (classic 'v1' (not v2) FLORA pixels, WS2811 drivers)
//   NEO_GRB     Pixels are wired for GRB bitstream (most NeoPixel products)
//   NEO_RGB     Pixels are wired for RGB bitstream (v1 FLORA pixels, not v2)
Adafruit_NeoPixel leds = Adafruit_NeoPixel(60, PIN, NEO_GRB + NEO_KHZ800);

void setup() 
{
	leds.begin();  // Call this to start up the LED strip.
	clearLEDs();   // This function, defined below, turns all LEDs off...
	leds.show();   // ...but the LEDs don't actually update until you call this.
}

void loop() {
	// Some example procedures showing how to display to the pixels:
	colorWipe(leds.Color(255, 0, 0), 50); // Red
	colorWipe(leds.Color(0, 255, 0), 50); // Green
	colorWipe(leds.Color(0, 0, 255), 50); // Blue
	rainbow(20);
	rainbowCycle(20);
}

// Fill the dots one after the other with a color
void colorWipe(uint32_t c, uint8_t wait) 
{
	for(uint16_t i=0; i<leds.numPixels(); i++) 
	{
		leds.setPixelColor(i, c);
		leds.show();
		delay(wait);
	}
}

void rainbow(uint8_t wait) 
{
	uint16_t i, j;

	for(j=0; j<256; j++) 
	{
		for(i=0; i<leds.numPixels(); i++) 
		{
			leds.setPixelColor(i, Wheel((i+j) & 255));
		}
		leds.show();
		delay(wait);
	}
}

// Slightly different, this makes the rainbow equally distributed throughout
void rainbowCycle(uint8_t wait) 
{
	uint16_t i, j;

	for(j=0; j<256*5; j++) 
	{ // 5 cycles of all colors on wheel
		for(i=0; i< leds.numPixels(); i++)
		{
			leds.setPixelColor(i, Wheel(((i * 256 / leds.numPixels()) + j) & 255));
		}
		leds.show();
		delay(wait);
	}
}

// Implements a little larson "cylon" sanner.
// This'll run one full cycle, down one way and back the other
void cylon(unsigned long color, byte wait)
{
	// weight determines how much lighter the outer "eye" colors are
	const byte weight = 4;  
	// It'll be easier to decrement each of these colors individually
	// so we'll split them out of the 24-bit color value
	byte red = (color & 0xFF0000) >> 16;
	byte green = (color & 0x00FF00) >> 8;
	byte blue = (color & 0x0000FF);

	// Start at closest LED, and move to the outside
	for (int i=0; i<=LED_COUNT-1; i++)
	{
		clearLEDs();
		leds.setPixelColor(i, red, green, blue);  // Set the bright middle eye
		// Now set two eyes to each side to get progressively dimmer
		for (int j=1; j<3; j++)
		{
			if (i-j >= 0)
				leds.setPixelColor(i-j, red/(weight*j), green/(weight*j), blue/(weight*j));
			if (i-j <= LED_COUNT)
				leds.setPixelColor(i+j, red/(weight*j), green/(weight*j), blue/(weight*j));
		}
		leds.show();  // Turn the LEDs on
		delay(wait);  // Delay for visibility
	}

	// Now we go back to where we came. Do the same thing.
	for (int i=LED_COUNT-2; i>=1; i--)
	{
		clearLEDs();
		leds.setPixelColor(i, red, green, blue);
		for (int j=1; j<3; j++)
		{
			if (i-j >= 0)
				leds.setPixelColor(i-j, red/(weight*j), green/(weight*j), blue/(weight*j));
			if (i-j <= LED_COUNT)
				leds.setPixelColor(i+j, red/(weight*j), green/(weight*j), blue/(weight*j));
		}

		leds.show();
		delay(wait);
	}
}

// Cascades a single direction. One time.
void cascade(unsigned long color, byte direction, byte wait)
{
	if (direction == TOP_DOWN)
	{
		for (int i=0; i<LED_COUNT; i++)
		{
			clearLEDs();  // Turn off all LEDs
			leds.setPixelColor(i, color);  // Set just this one
			leds.show();
			delay(wait);
		}
	}
	else
	{
		for (int i=LED_COUNT-1; i>=0; i--)
		{
			clearLEDs();
			leds.setPixelColor(i, color);
			leds.show();
			delay(wait);
		}
	}
}

// Sets all LEDs to off, but DOES NOT update the display;
// call leds.show() to actually turn them off after this.
void clearLEDs()
{
	for (int i=0; i<LED_COUNT; i++)
	{
		leds.setPixelColor(i, 0);
	}
}

// Prints a rainbow on the ENTIRE LED strip.
//  The rainbow begins at a specified position. 
// ROY G BIV!
void rainbow(byte startPosition) 
{
	// Need to scale our rainbow. We want a variety of colors, even if there
	// are just 10 or so pixels.
	int rainbowScale = 192 / LED_COUNT;

	// Next we setup each pixel with the right color
	for (int i=0; i<LED_COUNT; i++)
	{
		// There are 192 total colors we can get out of the rainbowOrder function.
		// It'll return a color between red->orange->green->...->violet for 0-191.
		leds.setPixelColor(i, rainbowOrder((rainbowScale * (i + startPosition)) % 192));
	}
	// Finally, actually turn the LEDs on:
	leds.show();
}

// Input a value 0 to 191 to get a color value.
// The colors are a transition red->yellow->green->aqua->blue->fuchsia->red...
//  Adapted from Wheel function in the Adafruit_NeoPixel library example sketch
uint32_t rainbowOrder(byte position) 
{
	// 6 total zones of color change:
	if (position < 31)  // Red -> Yellow (Red = FF, blue = 0, green goes 00-FF)
	{
		return leds.Color(0xFF, position * 8, 0);
	}
	else if (position < 63)  // Yellow -> Green (Green = FF, blue = 0, red goes FF->00)
	{
		position -= 31;
		return leds.Color(0xFF - position * 8, 0xFF, 0);
	}
	else if (position < 95)  // Green->Aqua (Green = FF, red = 0, blue goes 00->FF)
	{
		position -= 63;
		return leds.Color(0, 0xFF, position * 8);
	}
	else if (position < 127)  // Aqua->Blue (Blue = FF, red = 0, green goes FF->00)
	{
		position -= 95;
		return leds.Color(0, 0xFF - position * 8, 0xFF);
	}
	else if (position < 159)  // Blue->Fuchsia (Blue = FF, green = 0, red goes 00->FF)
	{
		position -= 127;
		return leds.Color(position * 8, 0, 0xFF);
	}
	else  //160 <position< 191   Fuchsia->Red (Red = FF, green = 0, blue goes FF->00)
	{
		position -= 159;
		return leds.Color(0xFF, 0x00, 0xFF - position * 8);
	}
}