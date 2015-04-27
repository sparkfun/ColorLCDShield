/*
  LCDShield.cpp - Arduino Library to control a Nokia 6100 LCD, 
  specifically that found on SparkFun's Color LCD Shield.
  This code should work for both Epson and Phillips display drivers 
  normally found on the Color LCD Shield.
	
  License: CC BY-SA 3.0: Creative Commons Share-alike 3.0. Feel free 
  to use and abuse this code however you'd like. If you find it useful
  please attribute, and SHARE-ALIKE!
  
  This is based on code by Mark Sproul, and Peter Davenport.
  Thanks to Coleman Sellers and Harold Timmis for help getting it to work with the Phillips Driver 7-31-2011
*/

#include "SparkFunColorLCDShield.h"

/*extern "C" {
	#include "wiring.h"
}*/

#include "Arduino.h"

static char x_offset = 0;
static char y_offset = 0;

LCDShield::LCDShield()
{

#if defined(__AVR_ATmega2560__) || defined(__AVR_ATmega1280__)
	DDRB = ((1<<LCD_PIN_DIO)|(1<<LCD_PIN_SCK));     //Set DIO and SCK pins on PORTB as outputs
	DDRH = ((1<<LCD_PIN_CS)|(1<<LCD_PIN_RES));  //Set CS and RES pins PORTH as outputs
#elif defined(__AVR_ATmega32U4__)
	DDRB = (1<<LCD_PIN_RES) | (1<<LCD_PIN_CS) | (1<<LCD_PIN_DIO);
	DDRC = (1<<LCD_PIN_SCK);
#else
	DDRB = ((1<<LCD_PIN_CS)|(1<<LCD_PIN_DIO)|(1<<LCD_PIN_SCK)|(1<<LCD_PIN_RES));  //Set the control pins as outputs
#endif

	DDRD	=	0x00;
	PORTD	=	0xFF;
}

void LCDShield::LCDCommand(unsigned char data)
{
	char jj;

	cbi(LCD_PORT_CS, LCD_PIN_CS);     // enable chip
	cbi(LCD_PORT_DIO, LCD_PIN_DIO);   // output low on data out (9th bit low = command)

	cbi(LCD_PORT_SCK, LCD_PIN_SCK);   // send clock pulse
	delayMicroseconds(1);
	sbi(LCD_PORT_SCK, LCD_PIN_SCK);

	for (jj = 0; jj < 8; jj++)
	{
		if ((data & 0x80) == 0x80)
			sbi(LCD_PORT_DIO, LCD_PIN_DIO);
		else
			cbi(LCD_PORT_DIO, LCD_PIN_DIO);

		cbi(LCD_PORT_SCK, LCD_PIN_SCK); // send clock pulse
		delayMicroseconds(1);
		sbi(LCD_PORT_SCK, LCD_PIN_SCK);

		data <<= 1;
	}

	sbi(LCD_PORT_CS, LCD_PIN_CS);     // disable
}

void LCDShield::LCDData(unsigned char data)
{
	char j;

	cbi(LCD_PORT_CS, LCD_PIN_CS);     // enable chip
	sbi(LCD_PORT_DIO, LCD_PIN_DIO);   // output high on data out (9th bit high = data)

	cbi(LCD_PORT_SCK, LCD_PIN_SCK);   // send clock pulse
	delayMicroseconds(1);
	sbi(LCD_PORT_SCK, LCD_PIN_SCK);   // send clock pulse

	for (j = 0; j < 8; j++)
	{
		if ((data & 0x80) == 0x80)
			sbi(LCD_PORT_DIO, LCD_PIN_DIO);
		else
			cbi(LCD_PORT_DIO, LCD_PIN_DIO);
	
		cbi(LCD_PORT_SCK, LCD_PIN_SCK); // send clock pulse
		delayMicroseconds(1);
		sbi(LCD_PORT_SCK, LCD_PIN_SCK);

		data <<= 1;
	}

	LCD_PORT_CS	|=	(1<<LCD_PIN_CS);  // disable
}

void LCDShield::init(int type, bool colorSwap)
{
	driver = type;
	
	// Initialize the control pins, and reset display:
	cbi(LCD_PORT_SCK, LCD_PIN_SCK);	// CLK = LOW
	cbi(LCD_PORT_DIO, LCD_PIN_DIO);		// DIO = LOW
	delayMicroseconds(10);		// 10us delay
	sbi(LCD_PORT_CS, LCD_PIN_CS);		// CS = HIGH
	delayMicroseconds(10);		// 10uS Delay
	cbi(LCD_PORT_RES, LCD_PIN_RES);	// RESET = LOW
	delay(200);					// 200ms delay
	sbi(LCD_PORT_RES, LCD_PIN_RES);	// RESET = HIGH
	delay(200);					// 200ms delay
	sbi(LCD_PORT_SCK, LCD_PIN_SCK);	// SCK = HIGH
	sbi(LCD_PORT_DIO, LCD_PIN_DIO);		// DIO = HIGH
	delayMicroseconds(10);		// 10us delay
	
	if (driver == EPSON)
	{
		LCDCommand(DISCTL);	// Display control (0xCA)
		LCDData(0x0C);		// 12 = 1100 - CL dividing ratio [don't divide] switching period 8H (default)
		LCDData(0x20);		// nlines/4 - 1 = 132/4 - 1 = 32 duty
		LCDData(0x00);		// No inversely highlighted lines
		
		LCDCommand(COMSCN);	// common scanning direction (0xBB)
		LCDData(0x01);		// 1->68, 132<-69 scan direction
		
		LCDCommand(OSCON);	// internal oscialltor ON (0xD1)
		LCDCommand(SLPOUT);	// sleep out (0x94)
		
		LCDCommand(PWRCTR);	// power ctrl (0x20)
		LCDData(0x0F);		// everything on, no external reference resistors
		
		LCDCommand(DISINV);	// invert display mode (0xA7)
		
		LCDCommand(DATCTL);	// data control (0xBC)
		LCDData(0x03);		// Inverse page address, reverse rotation column address, column scan-direction	!!! try 0x01
		LCDData(0x00);		// normal RGB arrangement
		LCDData(0x02);		// 16-bit Grayscale Type A (12-bit color)
		
		LCDCommand(VOLCTR);	// electronic volume, this is the contrast/brightness (0x81)
		LCDData(32);		// volume (contrast) setting - fine tuning, original (0-63)
		LCDData(3);			// internal resistor ratio - coarse adjustment (0-7)
		
		LCDCommand(NOP);	// nop (0x25)

		delay(100);

		LCDCommand(DISON);	// display on (0xAF)
	}
	else if (driver == PHILIPS)
	{
		LCDCommand(SLEEPOUT);	// Sleep Out (0x11)
		LCDCommand(BSTRON);   	// Booster voltage on (0x03)
		LCDCommand(DISPON);		// Display on (0x29)
		
		//LCDCommand(INVON);		// Inversion on (0x20)
		
		// 12-bit color pixel format:
		LCDCommand(COLMOD);		// Color interface format (0x3A)
		LCDData(0x03);			// 0b011 is 12-bit/pixel mode
		
		LCDCommand(MADCTL);		// Memory Access Control(PHILLIPS)
		if (colorSwap) 
			LCDData(0x08);
		else
			LCDData(0x00);
		
		LCDCommand(SETCON);		// Set Contrast(PHILLIPS)
		LCDData(0x30);
		
		LCDCommand(NOPP);		// nop(PHILLIPS)
	}
}

void LCDShield::clear(int color)
{
	if (driver) // if it's an Epson
	{
		LCDCommand(PASET);
		LCDData(0);
		LCDData(131);

		LCDCommand(CASET);
		LCDData(0);
		LCDData(131);

		LCDCommand(RAMWR);
	}
	else // otherwise it's a phillips
	{
		LCDCommand(PASETP);
		LCDData(0);
		LCDData(131);

		LCDCommand(CASETP);
		LCDData(0);
		LCDData(131);

		LCDCommand(RAMWRP);
	}

	for(unsigned int i=0; i < (131*131)/2; i++)
	{
		LCDData((color>>4)&0x00FF);
		LCDData(((color&0x0F)<<4)|(color>>8));
		LCDData(color&0x0FF);
	}

	x_offset = 0;
	y_offset = 0;
}

void LCDShield::contrast(char setting)
{
	if (driver == EPSON)
	{
		setting &= 0x3F;	// 2 msb's not used, mask out
		LCDCommand(VOLCTR);	// electronic volume, this is the contrast/brightness(EPSON)
		LCDData(setting);	// volume (contrast) setting - course adjustment,  -- original was 24
		LCDData(3);			// TODO: Make this coarse adjustment variable, 3's a good place to stay
	}
	else if (driver == PHILIPS)
	{
		setting &= 0x7F;	// msb is not used, mask it out
		LCDCommand(SETCON);	// contrast command (PHILLIPS)
		LCDData(setting);	// volume (contrast) setting - course adjustment,  -- original was 24
	}
}

// Added by Steve Sparks @ Big Nerd Ranch.
// This swaps the Epson RGB order into the Philips RGB order. (Or, vice versa, I suppose.)
uint16_t LCDShield::swapColors(uint16_t in) {
    return ((in & 0x000F)<<8)|(in & 0x00F0)|((in & 0x0F00)>>8);
}

void LCDShield::setPixel(int color, unsigned char x, unsigned char y)
{
	y	=	(COL_HEIGHT - 1) - y;
	x = (ROW_LENGTH - 1) - x;

	if (driver == EPSON) // if it's an epson
	{
		LCDCommand(PASET);  // page start/end ram
		LCDData(x);
		LCDData(ENDPAGE);

		LCDCommand(CASET);  // column start/end ram
		LCDData(y);
		LCDData(ENDCOL);

		LCDCommand(RAMWR);  // write
		LCDData((color>>4)&0x00FF);
		LCDData(((color&0x0F)<<4)|(color>>8));
		LCDData(color&0x0FF);
	}
	else if (driver == PHILIPS)  // otherwise it's a phillips
	{
		LCDCommand(PASETP); // page start/end ram
		LCDData(x);
		LCDData(x);

		LCDCommand(CASETP); // column start/end ram
		LCDData(y);
		LCDData(y);

		LCDCommand(RAMWRP); // write

		LCDData((unsigned char)((color>>4)&0x00FF));
		LCDData((unsigned char)(((color&0x0F)<<4)|0x00));
	}
}
// 2/18/2013 This Methos added by Tony Contrada in order to create arc segments in varied line thickness, or Filled
void LCDShield::setArc(int x0, int y0, int radius, int arcSegments[], int numSegments, int lineThickness, int color)
{
	//Line Thickness (Num Pixels)
	if(lineThickness == FILL) lineThickness = radius;
	for(int i = 0; i < lineThickness; i++)
	{
		int f = 1 - radius;
		int ddF_x = 0;
		int ddF_y = -2 * radius;
		int x = 0;
		int y = radius;
		while(x < y)
		{
			if(f >= 0)
			{
				y--;
				ddF_y += 2;
				f += ddF_y;
			}
			x++;
			ddF_x += 2;
			f += ddF_x + 1;

			for(int i = 0; i < numSegments; i++)
			{
				if(arcSegments[i] == NNE) setPixel(color, x0 - y, y0 + x); //SHOW NNE
				if(arcSegments[i] == ENE) setPixel(color, x0 - x, y0 + y); //SHOW ENE
				if(arcSegments[i] == ESE) setPixel(color, x0 + x, y0 + y); //SHOW ESE
				if(arcSegments[i] == SSE) setPixel(color, x0 + y, y0 + x); //SHOW SSE
				if(arcSegments[i] == SSW) setPixel(color, x0 + y, y0 - x); //SHOW SSW
				if(arcSegments[i] == WSW) setPixel(color, x0 + x, y0 - y); //SHOW WSW 
				if(arcSegments[i] == WNW) setPixel(color, x0 - x, y0 - y); //SHOW WNW
				if(arcSegments[i] == NNW) setPixel(color, x0 - y, y0 - x); //SHOW NNW
			}
		
		}
		radius--;
	}

}

// 2/22/2013 - Modified by Tony Contrada to include Line Thickness (in pixels) or a Filled Circle
void LCDShield::setCircle (int x0, int y0, int radius, int color, int lineThickness)
{
	if(lineThickness == FILL) lineThickness = radius;
	
	for(int r = 0; r < lineThickness; r++)
	{
		int f = 1 - radius;
		int ddF_x = 0;
		int ddF_y = -2 * radius;
		int x = 0;
		int y = radius;

		setPixel(color, x0, y0 + radius);
		setPixel(color, x0, y0 - radius);
		setPixel(color, x0 + radius, y0);
		setPixel(color, x0 - radius, y0);

		while(x < y)
		{
			if(f >= 0)
			{
				y--;
				ddF_y += 2;
				f += ddF_y;
			}
			x++;
			ddF_x += 2;
			f += ddF_x + 1;

			setPixel(color, x0 + x, y0 + y);
			setPixel(color, x0 - x, y0 + y);
			setPixel(color, x0 + x, y0 - y);
			setPixel(color, x0 - x, y0 - y);
			setPixel(color, x0 + y, y0 + x);
			setPixel(color, x0 - y, y0 + x);
			setPixel(color, x0 + y, y0 - x);
			setPixel(color, x0 - y, y0 - x);
		}
		radius--;
	}

}

void LCDShield::setChar(char c, int x, int y, int fColor, int bColor)
{
	y	=	(COL_HEIGHT - 1) - y; // make display "right" side up
	x	=	(ROW_LENGTH - 2) - x;

	int             i,j;
	unsigned int    nCols;
	unsigned int    nRows;
	unsigned int    nBytes;
	unsigned char   PixelRow;
	unsigned char   Mask;
	unsigned int    Word0;
	unsigned int    Word1;
	const unsigned char   *pFont;
	const unsigned char   *pChar;

	// get pointer to the beginning of the selected font table
	pFont = (const unsigned char *)FONT8x16;
	// get the nColumns, nRows and nBytes
	nCols = pgm_read_byte(pFont);
	nRows = pgm_read_byte(pFont + 1);
	nBytes = pgm_read_byte(pFont + 2);
	// get pointer to the last byte of the desired character
	pChar = pFont + (nBytes * (c - 0x1F)) + nBytes - 1;

	if (driver)	// if it's an epson
	{
		// Row address set (command 0x2B)
		LCDCommand(PASET);
		LCDData(x);
		LCDData(x + nRows - 1);
		// Column address set (command 0x2A)
		LCDCommand(CASET);
		LCDData(y);
		LCDData(y + nCols - 1);
	
		// WRITE MEMORY
		LCDCommand(RAMWR);
		// loop on each row, working backwards from the bottom to the top
		for (i = nRows - 1; i >= 0; i--) {
			// copy pixel row from font table and then decrement row
			PixelRow = pgm_read_byte(pChar++);
			// loop on each pixel in the row (left to right)
			// Note: we do two pixels each loop
			Mask = 0x80;
			for (j = 0; j < nCols; j += 2) 
			{
				// if pixel bit set, use foreground color; else use the background color
				// now get the pixel color for two successive pixels
				if ((PixelRow & Mask) == 0)
					Word0 = bColor;
				else
					Word0 = fColor;
				Mask = Mask >> 1;
				if ((PixelRow & Mask) == 0)
					Word1 = bColor;
				else
					Word1 = fColor;
				Mask = Mask >> 1;
				// use this information to output three data bytes
				LCDData((Word0 >> 4) & 0xFF);
				LCDData(((Word0 & 0xF) << 4) | ((Word1 >> 8) & 0xF));
				LCDData(Word1 & 0xFF);
			}
		}
	}
	else
	{
        fColor = swapColors(fColor);
        bColor = swapColors(bColor);

		// Row address set (command 0x2B)
		LCDCommand(PASETP);
		LCDData(x);
		LCDData(x + nRows - 1);
		// Column address set (command 0x2A)
		LCDCommand(CASETP);
		LCDData(y);
		LCDData(y + nCols - 1);
	
		// WRITE MEMORY
		LCDCommand(RAMWRP);
		// loop on each row, working backwards from the bottom to the top
		pChar+=nBytes-1;  // stick pChar at the end of the row - gonna reverse print on phillips
		for (i = nRows - 1; i >= 0; i--) {
			// copy pixel row from font table and then decrement row
			PixelRow = pgm_read_byte(pChar--);
			// loop on each pixel in the row (left to right)
			// Note: we do two pixels each loop
			Mask = 0x01;  // <- opposite of epson
			for (j = 0; j < nCols; j += 2) 
			{
				// if pixel bit set, use foreground color; else use the background color
				// now get the pixel color for two successive pixels
				if ((PixelRow & Mask) == 0)
					Word0 = bColor;
				else
					Word0 = fColor;
				Mask = Mask << 1; // <- opposite of epson
				if ((PixelRow & Mask) == 0)
					Word1 = bColor;
				else
					Word1 = fColor;
				Mask = Mask << 1; // <- opposite of epson
				// use this information to output three data bytes
				LCDData((Word0 >> 4) & 0xFF);
				LCDData(((Word0 & 0xF) << 4) | ((Word1 >> 8) & 0xF));
				LCDData(Word1 & 0xFF);
			}
		}
	}
}


void LCDShield::setStr(char *pString, int x, int y, int fColor, int bColor)
{
	x = x + 16;
	y = y + 8;
    int originalY = y;

	// loop until null-terminator is seen
	while (*pString != 0x00) {
		// draw the character
		setChar(*pString++, x, y, fColor, bColor);
		// advance the y position
		y = y + 8;
		// bail out if y exceeds 131
		if (y > 131) {
            x = x + 16;
            y = originalY;
        }
        if (x > 123) break;
	}
}

void LCDShield::setLine(int x0, int y0, int x1, int y1, int color)
{
	int dy = y1 - y0; // Difference between y0 and y1
	int dx = x1 - x0; // Difference between x0 and x1
	int stepx, stepy;

	if (dy < 0)
	{
		dy = -dy;
		stepy = -1;
	}
	else
		stepy = 1;

	if (dx < 0)
	{
		dx = -dx;
		stepx = -1;
	}
	else
		stepx = 1;

	dy <<= 1; // dy is now 2*dy
	dx <<= 1; // dx is now 2*dx
	setPixel(color, x0, y0);

	if (dx > dy) 
	{
		int fraction = dy - (dx >> 1);
		while (x0 != x1)
		{
			if (fraction >= 0)
			{
				y0 += stepy;
				fraction -= dx;
			}
			x0 += stepx;
			fraction += dy;
			setPixel(color, x0, y0);
		}
	}
	else
	{
		int fraction = dx - (dy >> 1);
		while (y0 != y1)
		{
			if (fraction >= 0)
			{
				x0 += stepx;
				fraction -= dy;
			}
			y0 += stepy;
			fraction += dx;
			setPixel(color, x0, y0);
		}
	}
}

void LCDShield::setRect(int x0, int y0, int x1, int y1, unsigned char fill, int color)
{
	// check if the rectangle is to be filled
	if (fill == 1)
	{
		int xDiff;
	
		if(x0 > x1)
			xDiff = x0 - x1; //Find the difference between the x vars
		else
			xDiff = x1 - x0;
	
		while(xDiff > 0)
		{
			setLine(x0, y0, x0, y1, color);
		
			if(x0 > x1)
				x0--;
			else
				x0++;
		
			xDiff--;
		}

	}
	else 
	{
		// best way to draw an unfilled rectangle is to draw four lines
		setLine(x0, y0, x1, y0, color);
		setLine(x0, y1, x1, y1, color);
		setLine(x0, y0, x0, y1, color);
		setLine(x1, y0, x1, y1, color);
	}
}

void LCDShield::printLogo(void)
{
	int x = 4, y = 25, logo_ix = 0, z;
	char logo;

	for (logo_ix = 0; logo_ix < 1120; logo_ix++)
	{
		logo = logo_spark[logo_ix];
		for (z = 0; z < 8; z++)
		{
			if ((logo & 0x80) == 0x80) setPixel(RED, y, x);
			x++;
			if (x == 132)
			{
				x = 4;
				y++;
			}
			logo <<= 1;
		}
	}
}

void LCDShield::printBMP(char image_main[2048]){ //prints an image (BMP)
    int x = 4, y = 0, image_ix = 0, z; //starts on row 0
    char image;
    
    for (image_ix = 0; image_ix < 2048; image_ix++){ //for all the bits in the image
        image = image_main[image_ix];
        
        for (z = 0; z <8; z++){ //for each bit in the byte
            if ((image & 0x80) == 0x80){ //if the bit is equal to 0, sets to white
                setPixel(WHITE, y, x);
            } else {
                setPixel(BLUE, y, x); //else sets it to black
            }
            x++; //adds one to x
            if (x == 132){ //if the end of the screen is reached
                x = 4; //sets x to 4 so not to be missed on edge of screen
                y++; //goes down a row
            }
            image <<= 1; //resets logo
        }
    }
}

void LCDShield::off(void)
{
	if (driver)	// If it's an epson
		LCDCommand(DISOFF);
	else // otherwise it's a phillips
		LCDCommand(DISPOFF);
}

void LCDShield::on(void)
{
	if (driver)	// If it's an epson
		LCDCommand(DISON);
	else // otherwise it's a phillips
		LCDCommand(DISPON);
}

