/*************************************
Functions
*************************************/

void off()
{
	clearpixel();   // This function, defined below, turns all pixel off...
	pixel.show();   // ...but the pixel don't actually update until you call this.
}

void allLEDS(unsigned long color)
{
	int color1 = CRGB::DarkViolet;
	leds[0] = color; 
	leds[1] = color;
	leds[2] = color;
	leds[3] = color;
	leds[4] = color;
	leds[5] = color;
	leds[6] = color;
	leds[7] = color;
	FastLED.show();

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

void clearpixel()
{
	for (int i=0; i<LED_COUNT; i++)
	{
		pixel.setPixelColor(i, 0);
	}
}

void colorFlush()
{
	colorWipe(pixel.Color(255, 0, 0), 50); // Red
	colorWipe(pixel.Color(0, 255, 0), 50); // Green
	colorWipe(pixel.Color(0, 0, 255), 50); // Blue
}

void colorWipe(uint32_t c, uint8_t wait) 
{
	for(uint16_t i=0; i<pixel.numPixels(); i++) 
	{
		pixel.setPixelColor(i, c);
		pixel.show();
		delay(wait);
	}
}

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

void dimmer()
{
	colorWipe(pixel.Color(10, 10, 10), 50);
}

void dimmer1()
{
	colorWipe(pixel.Color(5, 5, 5), 50);
}


