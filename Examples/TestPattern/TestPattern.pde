/*
  TestPattern - An example sketch for the Color LCD Shield Library
  by: Jim Lindblom
  SparkFun Electronics
  date: 6/23/11
  license: CC-BY SA 3.0 - Creative commons share-alike 3.0
  use this code however you'd like, just keep this license and
  attribute. Let me know if you make hugely, awesome, great changes.

  This sketch has example usage of the Color LCD Shield's three
  buttons. It also shows how to use the setRect and contrast
  functions.
  
  Hit S1 to increase the contrast, S2 decreases the contrast, and
  S3 sets the contrast back to the middle.
*/
#include <ColorLCDShield.h>

LCDShield lcd;

int buttons[3] = {3, 4, 5};  // S1 = 3, S2 = 4, S3 = 5
byte cont = 40;  // Good center value for contrast

void setup()
{
  for (int i=0; i<3; i++)
  {
    pinMode(buttons[i], INPUT);  // Set buttons as inputs
    digitalWrite(buttons[i], HIGH);  // Activate internal pull-up
  }
  
  lcd.init(EPSON);  // Initialize the LCD, try using PHILLIPS if it's not working
  lcd.contrast(cont);  // Initialize contrast
  lcd.clear(WHITE);  // Set background to white
  lcd.printLogo();  // Print SparkFun test logo
  testPattern();  // Print color bars on bottom of screen
}

void loop()
{
  while(digitalRead(buttons[0])&&digitalRead(buttons[1])&&digitalRead(buttons[2]))
    ;  // Wait, do nothing, until a button is pressed
  if (!digitalRead(buttons[0]))  // If S1 is hit, increase contrast
  {
    cont++;
    if (cont >= 60)
      cont = 0;
  }
  else if (!digitalRead(buttons[1]))  // If s2 is hit, decrease contrast
  {
    cont--;
    if (cont >= 60)
      cont = 59;
  }
  else if (!digitalRead(buttons[2]))  // If S3 is hit, reset contrast
  {
    cont = 38;
  }
  lcd.contrast(cont);  // give LCD contrast command
  
  delay(100);  // Delay to give each button press a little more meaning
}

void testPattern()
{
  lcd.setRect(80, 2, 131, 19, 1, WHITE);
  lcd.setRect(80, 19, 131, 35, 1, YELLOW);
  lcd.setRect(80, 35, 131, 51, 1, CYAN);
  lcd.setRect(80, 51, 131, 67, 1, GREEN);
  lcd.setRect(80, 67, 131, 83, 1, MAGENTA);
  lcd.setRect(80, 83, 131, 99, 1, RED);
  lcd.setRect(80, 99, 131, 115, 1, BLUE);
  lcd.setRect(80, 115, 131, 131, 1, BLACK);
}
