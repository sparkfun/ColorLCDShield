/*
  ZanyCircles - An example sketch for the Color LCD Shield Library
  by: Jim Lindblom
  SparkFun Electronics
  date: 6/23/11
  license: CC-BY SA 3.0 - Creative commons share-alike 3.0
  use this code however you'd like, just keep this license and
  attribute. Let me know if you make hugely, awesome, great changes.
  
  This simple sketch shows how you can use setCircle and setPixel
  with the Color LCD Shield library. 
*/
#include <ColorLCDShield.h>

#define CIRCLES 10  // Number of zany circles in display
#define BACKGROUND ORANGE  // Color of background
#define FOREGROUND BLUE  // color of circles

int radius = 1;  // size of circles
int jump = 5;  // +/- of possible jump
int xCir[CIRCLES];  // center points of circles
int yCir[CIRCLES];  // cetner points of circles

LCDShield lcd;

void setup()
{
  lcd.init(PHILIPS);  // Try EPSON if this doesn't work. If colors are swapped try init(PHILIPS, 1)
  lcd.contrast(-51);  // Feel free to change this for visibility, values between 0 and 60
  lcd.clear(BACKGROUND);
  
  // Initilize all circles' center points
  for (int i=0; i<CIRCLES; i++)
  {
    //xCir[i] = random(2, 131);  // random starting points
    //yCir[i] = random(2, 131);
    xCir[i] = 66;  // start in the middle
    yCir[i] = 66;
    
    // Circles must be stuck inside the box:
    if (xCir[i] >= 131-radius)
      xCir[i] = 131-radius;
    if (xCir[i] <= radius)
      xCir[i] = radius;
    if (yCir[i] >= 131-radius)
      yCir[i] = 131-radius;
    if (yCir[i] <= radius)
      yCir[i] = radius;
  }
}

void loop()
{
  for (int i=0; i<CIRCLES; i++)
  {
    // add a random number to x, y
    xCir[i] += random(-jump, jump+1);
    yCir[i] += random(-jump, jump+1);
    
    // Circles must be stuck inside the box:
    if (xCir[i] >= 131-radius)
      xCir[i] = 131-radius;
    if (xCir[i] <= radius)
      xCir[i] = radius;
    if (yCir[i] >= 131-radius)
      yCir[i] = 131-radius;
    if (yCir[i] <= radius)
      yCir[i] = radius;
  }
  for (int i=0; i<CIRCLES; i++)
  {
    for (int j=1; j<=radius; j++)
    {
      lcd.setCircle(xCir[i], yCir[i], j, FOREGROUND);  // draw the new circle
      lcd.setPixel(FOREGROUND, xCir[i], yCir[i]);  // fill in the center of the new circle
    }
  }
  delay(50);  // Little delay for visibility
  for (int i=0; i<CIRCLES; i++)
  {   
    for (int j=1; j<=radius; j++)
    {
      lcd.setCircle(xCir[i], yCir[i], j, BACKGROUND);  // clear the circle
      lcd.setPixel(BACKGROUND, xCir[i], yCir[i]);  // clear center of circle
    }
  }
}