/********************************************************
FTCL
7/1/14
********************************************************/


#include "FastLED.h"
#include <Adafruit_NeoPixel.h>
#include "WS2812_Definitions.h"


#define PIN 6
#define LED_COUNT 8


// This is an array of leds.  One item for each led in your strip.
CRGB leds[LED_COUNT];
// Parameter 1 = number of pixels in pixel
// Parameter 2 = pin number (most are valid)
// Parameter 3 = pixel type flags, add together as needed:
//   NEO_KHZ800  800 KHz bitstream (most NeoPixel products w/WS2812 pixel)
//   NEO_KHZ400  400 KHz (classic 'v1' (not v2) FLORA pixels, WS2811 drivers)
//   NEO_GRB     Pixels are wired for GRB bitstream (most NeoPixel products)
//   NEO_RGB     Pixels are wired for RGB bitstream (v1 FLORA pixels, not v2)
Adafruit_NeoPixel pixel = Adafruit_NeoPixel(LED_COUNT, PIN, NEO_GRB + NEO_KHZ800);

void setup() 
{
	// sanity check delay - allows reprogramming if accidently blowing power w/leds
	delay(2000);

	// Uncomment one of the following lines for your leds arrangement.
	//FastLED.addLeds<WS2812, PIN, RGB>(leds, LED_COUNT);
	//FastLED.addLeds<WS2812B, PIN, RGB>(leds, LED_COUNT);
	FastLED.addLeds<NEOPIXEL, PIN, GRB>(leds, LED_COUNT);


	pixel.begin();  // Call this to start up the LED strip.
	clearpixel();   // This function, defined below, turns all pixel off...
	pixel.show();   // ...but the pixel don't actually update until you call this.
}

void loop() {
	// Some example procedures showing how to display to the pixels:
	colorFlush();
	rainbow(5);
	//rainbow2(5); // doesn't work
	rainbowCycle(5);
	firstLight();  // whit traveling dot
	//blink();
	cylon(BLUE,100); // (color,speed)
	cylon2();
	//calibrate();
	siren();
	//allLEDS();

}

void colorFlush()
{
	colorWipe(pixel.Color(255, 0, 0), 50); // Red
	colorWipe(pixel.Color(0, 255, 0), 50); // Green
	colorWipe(pixel.Color(0, 0, 255), 50); // Blue
}


// Fill the dots one after the other with a color	
//colorWipe(pixel.Color(255, 0, 0), 50); // Red
//colorWipe(pixel.Color(0, 255, 0), 50); // Green
//colorWipe(pixel.Color(0, 0, 255), 50); // Blue
void colorWipe(uint32_t c, uint8_t wait) 
{
	for(uint16_t i=0; i<pixel.numPixels(); i++) 
	{
		pixel.setPixelColor(i, c);
		pixel.show();
		delay(wait);
	}
}


void rainbow(uint8_t wait) 
{
	uint16_t i, j;
	for(j=0; j<256; j++) 
	{
		for(i=0; i<pixel.numPixels(); i++) 
		{
			pixel.setPixelColor(i, Wheel((i+j) & 255));
		}
		pixel.show();
		delay(wait);
	}
}

// Slightly different, this makes the rainbow equally distributed throughout
void rainbowCycle(uint8_t wait) 
{
	uint16_t i, j;

	for(j=0; j<256*5; j++) 
	{ // 5 cycles of all colors on wheel
		for(i=0; i< pixel.numPixels(); i++)
		{
			pixel.setPixelColor(i, Wheel(((i * 256 / pixel.numPixels()) + j) & 255));
		}
		pixel.show();
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
		clearpixel();
		pixel.setPixelColor(i, red, green, blue);  // Set the bright middle eye
		// Now set two eyes to each side to get progressively dimmer
		for (int j=1; j<3; j++)
		{
			if (i-j >= 0)
				pixel.setPixelColor(i-j, red/(weight*j), green/(weight*j), blue/(weight*j));
			if (i-j <= LED_COUNT)
				pixel.setPixelColor(i+j, red/(weight*j), green/(weight*j), blue/(weight*j));
		}
		pixel.show();  // Turn the pixel on
		delay(wait);  // Delay for visibility
	}

	// Now we go back to where we came. Do the same thing.
	for (int i=LED_COUNT-2; i>=1; i--)
	{
		clearpixel();
		pixel.setPixelColor(i, red, green, blue);
		for (int j=1; j<3; j++)
		{
			if (i-j >= 0)
				pixel.setPixelColor(i-j, red/(weight*j), green/(weight*j), blue/(weight*j));
			if (i-j <= LED_COUNT)
				pixel.setPixelColor(i+j, red/(weight*j), green/(weight*j), blue/(weight*j));
		}

		pixel.show();
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
			clearpixel();  // Turn off all pixel
			pixel.setPixelColor(i, color);  // Set just this one
			pixel.show();
			delay(wait);
		}
	}
	else
	{
		for (int i=LED_COUNT-1; i>=0; i--)
		{
			clearpixel();
			pixel.setPixelColor(i, color);
			pixel.show();
			delay(wait);
		}
	}
}

// Sets all pixel to off, but DOES NOT update the display;
// call pixel.show() to actually turn them off after this.
void clearpixel()
{
	for (int i=0; i<LED_COUNT; i++)
	{
		pixel.setPixelColor(i, 0);
	}
}

// Prints a rainbow on the ENTIRE LED strip.
//  The rainbow begins at a specified position. 
// ROY G BIV!
void rainbow2(byte startPosition) 
{
	// Need to scale our rainbow. We want a variety of colors, even if there
	// are just 10 or so pixels.
	int rainbowScale = 192 / LED_COUNT;

	// Next we setup each pixel with the right color
	for (int i=0; i<LED_COUNT; i++)
	{
		// There are 192 total colors we can get out of the rainbowOrder function.
		// It'll return a color between red->orange->green->...->violet for 0-191.
		pixel.setPixelColor(i, rainbowOrder((rainbowScale * (i + startPosition)) % 192));
	}
	// Finally, actually turn the pixel on:
	pixel.show();
}

// Input a value 0 to 191 to get a color value.
// The colors are a transition red->yellow->green->aqua->blue->fuchsia->red...
//  Adapted from Wheel function in the Adafruit_NeoPixel library example sketch
uint32_t rainbowOrder(byte position) 
{
	// 6 total zones of color change:
	if (position < 31)  // Red -> Yellow (Red = FF, blue = 0, green goes 00-FF)
	{
		return pixel.Color(0xFF, position * 8, 0);
	}
	else if (position < 63)  // Yellow -> Green (Green = FF, blue = 0, red goes FF->00)
	{
		position -= 31;
		return pixel.Color(0xFF - position * 8, 0xFF, 0);
	}
	else if (position < 95)  // Green->Aqua (Green = FF, red = 0, blue goes 00->FF)
	{
		position -= 63;
		return pixel.Color(0, 0xFF, position * 8);
	}
	else if (position < 127)  // Aqua->Blue (Blue = FF, red = 0, green goes FF->00)
	{
		position -= 95;
		return pixel.Color(0, 0xFF - position * 8, 0xFF);
	}
	else if (position < 159)  // Blue->Fuchsia (Blue = FF, green = 0, red goes 00->FF)
	{
		position -= 127;
		return pixel.Color(position * 8, 0, 0xFF);
	}
	else  //160 <position< 191   Fuchsia->Red (Red = FF, green = 0, blue goes FF->00)
	{
		position -= 159;
		return pixel.Color(0xFF, 0x00, 0xFF - position * 8);
	}
}
void firstLight()
{
	// Move a single white led 
	for(int whiteLed = 0; whiteLed < LED_COUNT; whiteLed = whiteLed + 1) 
	{
		// Turn our current led on to white, then show the leds
		leds[whiteLed] = CRGB::White;

		// Show the leds (only one of which is set to white, from above)
		FastLED.show();

		// Wait a little bit
		delay(100);

		// Turn our current led back to black for the next loop around
		leds[whiteLed] = CRGB::Black;
	}
}

void blink()
{
	// Turn the LED on, then pause
	leds[0] = CRGB::Blue;
	FastLED.show();
	delay(500);
	// Now turn the LED off, then pause
	leds[0] = CRGB::Black;
	FastLED.show();
	delay(500);

}
void cylon2()
{
	// First slide the led in one direction
	for(int i = 0; i < LED_COUNT; i++) 
	{
		// Set the i'th led to red 
		leds[i] = CRGB::DarkRed;
		// Show the leds
		FastLED.show();
		// now that we've shown the leds, reset the i'th led to black
		leds[i] = CRGB::Black;
		// Wait a little bit before we loop around and do it again
		delay(30);
	}

	// Now go in the other direction.  
	for(int i = LED_COUNT-1; i >= 0; i--) 
	{
		// Set the i'th led to red 
		leds[i] = CRGB::DarkRed;
		// Show the leds
		FastLED.show();
		// now that we've shown the leds, reset the i'th led to black
		leds[i] = CRGB::Black;
		// Wait a little bit before we loop around and do it again
		delay(30);
	}
}
void calibrate()
{
	leds[0] = CRGB::Thistle; 
	leds[1] = CRGB::Thistle;
	leds[2] = CRGB::Green;
	leds[3] = CRGB::Green;
	leds[4] = CRGB::Blue;
	leds[8] = CRGB::Blue;
	leds[6] = CRGB::Blue;
	leds[7] = CRGB::Blue;

	FastLED.show();
	delay(1000);
}

void allLEDS()
{

	leds[0] = CRGB::MediumBlue; 
	leds[1] = CRGB::SteelBlue;
	leds[2] = CRGB::Aquamarine;
	leds[3] = CRGB::DarkViolet;
	leds[4] = CRGB::DarkViolet;
	leds[5] = CRGB::DarkViolet;
	leds[6] = CRGB::DarkViolet;
	leds[7] = CRGB::DarkViolet;
	FastLED.show();
	delay(1000);
}

void siren()
{
	// First slide the led in one direction
	for(int i = 0; i < LED_COUNT; i++) 
	{
		// Set the i'th led to red 
		leds[i] = CRGB::Blue;
		// Show the leds
		FastLED.show();
		// now that we've shown the leds, reset the i'th led to black
		leds[i] = CRGB::Black;
		// Wait a little bit before we loop around and do it again
		delay(30);
	}

	// Now go in the other direction.  
	for(int i = LED_COUNT-1; i >= 0; i--) 
	{
		// Set the i'th led to red 
		leds[i] = CRGB::Red;
		// Show the leds
		FastLED.show();
		// now that we've shown the leds, reset the i'th led to black
		leds[i] = CRGB::Black;
		// Wait a little bit before we loop around and do it again
		delay(30);
	}
}
// Input a value 0 to 255 to get a color value.
// The colours are a transition r - g - b - back to r.
uint32_t Wheel(byte WheelPos) 
{
	if(WheelPos < 85) 
	{
		return pixel.Color(WheelPos * 3, 255 - WheelPos * 3, 0);
	} 
	else if(WheelPos < 170)
	{
		WheelPos -= 85;
		return pixel.Color(255 - WheelPos * 3, 0, WheelPos * 3);
	} 
	else 
	{
		WheelPos -= 170;
		return pixel.Color(0, WheelPos * 3, 255 - WheelPos * 3);
	}
}
