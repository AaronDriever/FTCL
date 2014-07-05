/********************************************************
FTCL
7/1/14
********************************************************/


#include <SoftwareSerial.h>
#include "FastLED.h"
#include <Adafruit_NeoPixel.h>
#include "WS2812_Definitions.h"

SoftwareSerial XBee(2, 3); // Arduino RX, TX (XBee Dout, Din)

#define PIN 6
#define LED_COUNT 8


// This is an array of leds.  One item for each led in your strip.
CRGB leds[LED_COUNT];
// Parameter 1 = number of pixels in pixel
// Parameter 2 = pin number (most are valid)
// Parameter 3 = pixel type flags, add together as needed:
//   NEO_KHZ800  800 KHz bitstream (most NeoPixel products w/WS2812 pixel)

Adafruit_NeoPixel pixel = Adafruit_NeoPixel(LED_COUNT, PIN, NEO_GRB + NEO_KHZ800);

void setup() 
{
	XBee.begin(115200); 
	Serial.begin(9600);
	// sanity check delay - allows reprogramming if accidently blowing power w/leds
	delay(50);

	// Uncomment one of the following lines for your leds arrangement.
	//FastLED.addLeds<WS2812, PIN, RGB>(leds, LED_COUNT);
	//FastLED.addLeds<WS2812B, PIN, RGB>(leds, LED_COUNT);
	FastLED.addLeds<NEOPIXEL, PIN, GRB>(leds, LED_COUNT);

	pixel.begin();  // Call this to start up the LED strip.
	clearpixel();   // This function, defined below, turns all pixel off...
	pixel.show();   // ...but the pixel don't actually update until you call this.
	printMenu();
}

void loop() 
{

	// read the sensor:
	if (Serial.available() > 0) 
	{
		int inByte = Serial.read();
		// do something different depending on the character received.  
		// The switch statement expects single number values for each case;
		// in this exmaple, though, you're using single quotes to tell
		// the controller to get the ASCII value for the character.  For 
		// example 'a' = 97, 'b' = 98, and so forth:

		allLEDS(inByte);

		/*
		switch (inByte) 
		{
		case '0':    
		off();
		break;		
		case '1':    
		allLEDS(RED);
		break;
		case '2':    
		allLEDS(MIDNIGHTBLUE);
		break;
		case '3':    
		allLEDS(DARKBLUE);
		break;
		case '4':    
		allLEDS(DARKGREEN);
		break;
		case '5':    
		allLEDS(YELLOW);
		break;
		case '6':    
		allLEDS(PURPLE);
		break;
		case '7':    
		allLEDS(WHITE);
		break;
		case '8':    
		allLEDS(DIMGRAY);
		break;
		case '9':    
		allLEDS(MEDIUMAQUAMARINE);
		break;
		case 'a':    
		dimmer();
		break;
		case 'b':    
		dimmer1();
		break;
		}*/
	}
}

void printMenu()
{
	// Everything is "F()"'d -- which stores the strings in flash.
	// That'll free up SRAM for more importanat stuff.
	Serial.println();
	Serial.println(F("Fish Tank Color Light Control!"));
	Serial.println(F("============================"));
	Serial.println(F("Usage: "));
	Serial.println(F("0 = Off"));
	Serial.println(F("1 = "));
	Serial.println(F("2 = "));
	Serial.println(F("3 = "));
	Serial.println(F("4 = "));
	Serial.println(F("5 = "));
	Serial.println(F("6 = "));
	Serial.println(F("7 = "));
	Serial.println(F("8 = "));
	Serial.println(F("9 = "));
	Serial.println(F(""));
	Serial.println(F(""));
	Serial.println(F(""));
	Serial.println(F(""));
	Serial.println(F("============================"));  
	Serial.println();
}



//Colors.
/*

BLACK=001			
NAVY=002		
DARKBLUE=003		
MEDIUMBLUE=004		
BLUE			
DARKGREEN		
GREEN			
TEAL			
DARKCYAN		
DEEPSKYBLUE		
DARKTURQUOISE		
MEDIUMSPRINGGREEN	
LIME			
SPRINGGREEN		
AQUA			
CYAN			
MIDNIGHTBLUE		
DODGERBLUE		
LIGHTSEAGREEN		
FORESTGREEN		
SEAGREEN		
DARKSLATEGRAY		
LIMEGREEN		
MEDIUMSEAGREEN		
TURQUOISE		
ROYALBLUE		
STEELBLUE		
DARKSLATEBLUE		
MEDIUMTURQUOISE		
INDIGO 			
DARKOLIVEGREEN		
CADETBLUE		
CORNFLOWERBLUE		
MEDIUMAQUAMARINE	
DIMGRAY			
SLATEBLUE		
OLIVEDRAB		
SLATEGRAY		
LIGHTSLATEGRAY		
MEDIUMSLATEBLUE		
LAWNGREEN		
CHARTREUSE		
AQUAMARINE		
MAROON			
PURPLE			
OLIVE			
GRAY			
SKYBLUE			
LIGHTSKYBLUE		
BLUEVIOLET		
DARKRED			
DARKMAGENTA		
SADDLEBROWN		
DARKSEAGREEN		
LIGHTGREEN		
MEDIUMPURPLE		
DARKVIOLET		
PALEGREEN		
DARKORCHID		
YELLOWGREEN		
SIENNA			
BROWN			
DARKGRAY		
LIGHTBLUE		
GREENYELLOW		
PALETURQUOISE	        
LIGHTSTEELBLUE		
POWDERBLUE		
FIREBRICK		
DARKGOLDENROD		
MEDIUMORCHID		
ROSYBROWN		
DARKKHAKI		
SILVER			
MEDIUMVIOLETRED	        
INDIANRED 		
PERU			
CHOCOLATE		
TAN			
LIGHTGRAY		
THISTLE			
ORCHID			
PALEVIOLETRED		
CRIMSON			
GAINSBORO		
PLUM			
BURLYWOOD		
LIGHTCYAN		
LAVENDER		
DARKSALMON		
VIOLET			
PALEGOLDENROD		
LIGHTCORAL		
KHAKI			
ALICEBLUE		
HONEYDEW		
AZURE			
SANDYBROWN		
WHEAT			
BEIGE			
WHITESMOKE		
MINTCREAM		
GHOSTWHITE		
SALMON			
ANTIQUEWHITE		
LINEN			
LIGHTGOLDENRODYELLOW	
OLDLACE			
RED			
FUCHSIA			
MAGENTA			
DEEPPINK		
ORANGERED		
TOMATO			
HOTPINK			
CORAL			
DARKORANGE		
LIGHTSALMON		
ORANGE			
LIGHTPINK		
PINK			
GOLD			
PEACHPUFF		
NAVAJOWHITE		
MOCCASIN		
BISQUE			
MISTYROSE		
BLANCHEDALMOND		
PAPAYAWHIP		
LAVENDERBLUSH		
SEASHELL		
CORNSILK		
LEMONCHIFFON		
FLORALWHITE		
SNOW			
YELLOW			
LIGHTYELLOW		
IVORY			
WHITE			
*/