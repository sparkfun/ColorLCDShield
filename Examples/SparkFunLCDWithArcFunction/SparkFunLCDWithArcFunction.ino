// This Sample Created by Tony Contrada (2/22/2013) for use with the Sparkfun Color LCD Shield
// with the updated Arduino Color LCD Library.
// This sample sketch illustrates the new setArf method and an updated setCircle method
// which includes setting a Line Thickness in Pixels and the ability to Fill-In the circle.

#include <SparkFunColorLCDShield.h>

LCDShield lcd;  // Creates an LCDShield, named lcd

void setup()
{
  lcd.init(PHILIPS);  // Initializes lcd, using an PHILIPSdriver
  lcd.contrast(-64);  // -51's usually a good contrast value
  lcd.clear(BLACK);  // clear the screen
  
  //Creates RED Arc in the ENE Quadrant with a Line Thickness of 5 Pixels
  int segmentsRed[1] = {ENE};
  lcd.setArc(60,50,40,segmentsRed,sizeof(segmentsRed),5,RED);
  
  //Creates YELLOW Arc in the NNE Quadrant with a Line Thickness of 10 Pixels
  int segmentsYellow[1] = {NNE};
  lcd.setArc(60,50,40,segmentsYellow,sizeof(segmentsYellow),10,YELLOW);
  
  //Creates GREEN Arc in the WNW and NNW Quadrants with a FILL
  int segments[2] = {WNW,NNW};
  lcd.setArc(60,50,40,segments,sizeof(segments),FILL,GREEN);
  
  //Creates PINK Circle with a FILL
  lcd.setCircle(90,100,20,PINK,FILL);
  
  //Creates CYAN Circle with a Line Thickness of 3 Pixels
  lcd.setCircle(90,35,25,CYAN,3);

}

void loop()
{
  
}


