# ColorLCDShield

This is a simple Arduino library for use specifically with SparkFun Electronics' [Color LCD Shield](http://www.sparkfun.com/products/9363). It'll no doubt work with other incarnations of LCD breakouts and what not, you just might have to do some modifying of the pin defintions.

## License
Creative Commons Share-alike 3.0 ([CC-BY SA 3.0](http://creativecommons.org/licenses/by-sa/3.0/)). Use this code however you'd like, just keep this license and attribute. Let us know if you make hugely, awesome, great changes.

## How to use the library
The code gives you access to the following LCD functions:

###LCDShield()
The library's constructor. An instance of the lcd must be created at the beginning of any code. e.g:

	LCDShield lcd;

###void init(int type, bool colorSwap)
This initializes an LCD. Either EPSON or PHILLIPS should be passed as the type variable. This function should be called near the beginning of any sketch (setup()!). Turns on the display, sets the contrast and initializes the LCD into 12-bit RGB color mode.

An optional variable, *colorSwap*, is useful if your blues and reds are swapped. By default this boolean variable is 0, and you don't have to specify it. Try making it 1 if you need to fix colors though (through my experience, I've seen Philips controllers go both ways.

Examples:

	lcd.init(PHILIPS);	// Initialize an LCD with a PHILIPS controller
	lcd.init(EPSON);	// Initialize and LCD with an EPSON controller
	lcd.init(PHILIPS, 1);	// Initialize with a PHILIPS controller, but swap all reds and blues.

###void clear(int color)
Clears the entire screen, filling it with the 12-bit RGB color passed to it. There are a number of predefined colors in *ColorLCDShield.h*.

###void contrast(char setting)
Manually adjusts the contrast. The setting variable is different for Philips or Epson LCDs.

For Philips, a signed value between -64 and 63 should be passed. I've found about -51 to work best.

For Epson, a value between 0 and 63 should be passed. 40 works pretty well for me.

###void setPixel(int color, unsigned char x, unsigned char y)
Draws a single pixel at the specified x, y location. Color is a 12-bit RGB value.

###void setArc(int x0, int y0, int radius, int segments[], int numSegments, int lineThickness, int color)
Draws an Arc centered at x0, y0 with the specified radius with the array of quadrant segments (NNE,ENE,ESE,SSE,SSW,WSW,WNW,NNW), number of quadrant segments, with the specified line thickness in pixels, or FILL to fill quadrant and color 12 bit RGB value.

###void setCircle (int x0, int y0, int radius, int color, int lineThickness)
Draws a circle centered around x0, y0, of the specified radius and color. Radius is in pixels, color is a 12-bit RGB value. LineThickness in Pixels or FILL to fill in circle (defaults to 1 pixel if not specified).

###void setChar(char c, int x, int y, int fColor, int bColor)
Sets a single character down at the specified x/y coordinate. You can pick both the foregroundand background color, they're 12-bit RGB values. Only one font is available in this library. Definitely room for growth, though at an added cost of memory.

###void setStr(char *pString, int x, int y, int fColor, int bColor)
Sets a string of characters down, beginning at x/y coordinates. You can pick both the foreground and background color, they're 12-bit RGB values. Only one font is available in this library. Definitely room for growth, though at an added cost of memory.

###void setLine(int x0, int y0, int x1, int y1, int color)
Draws a line from x0,y0 to x1,y1. Color is a 12-bit RGB value.

###void setRect(int x0, int y0, int x1, int y1, unsigned char fill, int color)
Draws a rectangle with opposing corners at x0,y0 and x1,y1. A 1 for fill will fill the entire rectangle, a 0 will only draw the border. Color is a 12-bit RGB value.

###void printLogo(void)
Prints the SparkFun logo. Good example of how to print a bitmap, though!

###void on(void)
Turns the display on.

###void off(void)
Turns the display off. You'll still see the backlight on!